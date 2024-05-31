module LevelManager;
import imgui;
import fmt;


LevelManager::Level::Level(const nlohmann::json &json)
{
  m_name = json.at("name").get<std::string>();
  m_tilemap_name = json.at("tilemap").get<std::string>();

  // Deserialize spawning zones
  for (const auto &zone : json.at("spawningzones"))
  {
    sf::IntRect rect(zone[0], zone[1], zone[2], zone[3]);
    m_spawning_zones.push_back(rect);
  }

  // Deserialize enemy waves
  for (const auto &wave : json.at("waves"))
  {
    spawn_data data;
    data.type = static_cast<components::enemy_type>(
      wave.at("type").get<int>());// Convert int to enemy_type enum
    data.count = wave.at("count").get<std::size_t>();
    data.time_for_next_wave = wave.at("time_for_next_wave").get<float>();
    data.delay_each = wave.at("delay_each").get<float>();
    m_waves.push_back(data);
  }
}


components::enemy_type LevelManager::Level::update(float delta)
{
  auto draw_debug = [&delta](const LevelManager::Level &level) {
    ImGui::Text("Delta: %f", delta);
    ImGui::Text("Name: %s", level.m_name.c_str());
    ImGui::Text("timer: %.2f", level.timer);
    ImGui::Text("delay: %.2f", level.m_waves.front().delay_each);
    ImGui::Text("Enemy waves:");

    for (const auto &wave : level.m_waves)
    {
      ImGui::Text("Type: %d, Count: %zu, next wave in: %f, Delay: %f",
        static_cast<int>(wave.type),
        wave.count,
        wave.time_for_next_wave,
        wave.delay_each);
      ImGui::Separator();
    }
  };

  draw_debug(*this);
  timer += delta;
  if (m_waves.empty()) { return components::enemy_type::none; }


  auto &data = m_waves.front();

  if (data.count > 0 && timer >= data.delay_each)
  {
    data.count--;
    timer = 0.f;
    return data.type;
  } else
  {
    // if no more enemies in this wave, wait for the next wave
    data.time_for_next_wave -= delta;
    if (data.time_for_next_wave <= 0.f) { m_waves.erase(m_waves.begin()); }
  }
  return components::enemy_type::none;
}

LevelManager::LevelManager(std::string_view levels_file, EntityFactory &factory)
  : m_factory(factory)
{
  my_assert(std::filesystem::exists(levels_file), "levels file does not exist");
  std::ifstream file(levels_file);
  if (!file.is_open())
  {
    my_assert(file.is_open(), "failed to open levels file");
  }
  m_levels_data = nlohmann::json::parse(file);

  load_levels();
}

void LevelManager::load_levels()
{
  for (auto &level : m_levels_data)
  {
    m_levels.emplace_back(level);
    // if()
  }
  m_current_level = &m_levels.front();
}

components::enemy_type LevelManager::update(float delta)
{
  // if (m_current_level->m_waves.empty())
  // {
  //   m_levels.erase(m_levels.begin());
  //   if (m_levels.empty()) { return components::enemy_type::none; }
  //   m_current_level = &m_levels.front();
  // }
  auto type = m_current_level->update(delta);
  return type;
}
