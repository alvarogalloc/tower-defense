module LevelManager;
import imgui;
import fmt;

// #if 0
// LevelManager::Level::Level(const nlohmann::json &json)
// {
//   m_name = json.at("name").get<std::string>();
//   m_tilemap_name = json.at("tilemap").get<std::string>();
//
//   // Deserialize enemy waves
//   for (const auto &wave : json.at("waves"))
//   {
//     spawn_data data;
//     data.type = static_cast<components::enemy_type>(
//       wave.at("type").get<int>());// Convert int to enemy_type enum
//     data.count = wave.at("count").get<std::size_t>();
//     data.time_for_next_wave = wave.at("time_for_next_wave").get<float>();
//     data.delay_each = wave.at("delay_each").get<float>();
//     m_waves.push_back(data);
//   }
// }
//
//
// components::enemy_type LevelManager::Level::update(float delta)
// {
//   auto draw_debug = [&delta](const LevelManager::Level &level) {
//     ImGui::Text("Delta: %f", delta);
//     ImGui::Text("Name: %s", level.m_name.c_str());
//     ImGui::Text("timer: %.2f", level.timer);
//     ImGui::Text("delay: %.2f", level.m_waves.front().delay_each);
//     ImGui::Text("Enemy waves:");
//
//     for (const auto &wave : level.m_waves)
//     {
//       ImGui::Text("Type: %d, Count: %zu, next wave in: %f, Delay: %f",
//         static_cast<int>(wave.type),
//         wave.count,
//         wave.time_for_next_wave,
//         wave.delay_each);
//       ImGui::Separator();
//     }
//   };
//
//   draw_debug(*this);
//   timer += delta;
//   if (m_waves.empty()) { return components::enemy_type::none; }
//
//
//   auto &data = m_waves.front();
//
//   if (data.count > 0 && timer >= data.delay_each)
//   {
//     data.count--;
//     timer = 0.f;
//     return data.type;
//   } else
//   {
//     // if no more enemies in this wave, wait for the next wave
//     data.time_for_next_wave -= delta;
//     if (data.time_for_next_wave <= 0.f) { m_waves.erase(m_waves.begin()); }
//   }
//   return components::enemy_type::none;
// }
//
// LevelManager::LevelManager(std::string_view levels_file, EntityFactory
// &factory)
//   : m_factory(factory)
// {
//   my_assert(std::filesystem::exists(levels_file), "levels file does not
//   exist"); auto file = std::unique_ptr<std::FILE, decltype(&std::fclose)>(
//     std::fopen(levels_file.data(), "r"), &std::fclose);
//   my_assert(bool(file), "failed to open levels file");
//   m_levels_data = nlohmann::json::parse(file.get());
//
//   load_levels();
// }
//
// void LevelManager::load_levels()
// {
//   for (auto &level : m_levels_data)
//   {
//     m_levels.emplace_back(level);
//     // if()
//   }
//   m_current_level = &m_levels.front();
// }
//
// components::enemy_type LevelManager::update(float delta)
// {
//   // if (m_current_level->m_waves.empty())
//   // {
//   //   m_levels.erase(m_levels.begin());
//   //   if (m_levels.empty()) { return components::enemy_type::none; }
//   //   m_current_level = &m_levels.front();
//   // }
//   auto type = m_current_level->update(delta);
//   return type;
// }
// #endif


LevelManager::Level::Level(const nlohmann::json &levelnode, my_assets &manager)
{
  m_name = levelnode.at("name").get<std::string>();
  m_tilemap_name = levelnode.at("tilemap").get<std::string>();
  fmt::print("Loaded tilemap: {}\n", m_tilemap_name);
  m_map.load(manager.get_path() + m_tilemap_name, manager);

  // Deserialize enemy waves
  for (const auto &wave : levelnode.at("waves"))
  {
    spawn_data data;
    data.type = static_cast<components::enemy_type>(
      wave.at("type").get<int>());// Convert int to enemy_type enum
    data.count = wave.at("count").get<std::size_t>();
    data.time_for_next_wave = wave.at("time_for_next_wave").get<float>();
    data.delay_each = wave.at("delay_each").get<float>();
    m_waves.push_back(data);
  }
  m_enemies_turning_points = get_points(m_map.get_map()->ly_head, "turningpoints");
}
components::enemy_type LevelManager::Level::update(float delta, bool draw_debug)
{
  auto draw_debug_gui = [&delta](const LevelManager::Level &level) {
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

  if (draw_debug) draw_debug_gui(*this);
  timer += delta;
  if (m_waves.empty()) { return components::enemy_type::none; }


  auto &data = m_waves.front();

  if (data.count > 0 && timer >= data.delay_each)
  {
    data.count--;
    timer = 0.f;
    return data.type;
  } else if (data.count == 0)
  {
    // if no more enemies in this wave, wait for the next wave
    data.time_for_next_wave -= delta;
    if (data.time_for_next_wave <= 0.f) { m_waves.erase(m_waves.begin()); }
  }
  return components::enemy_type::none;
}


LevelManager::LevelManager(nlohmann::json levels_json,
  nlohmann::json enemies_json,
  my_assets &manager)
  : m_manager(&manager)
{
  m_enemy_factory = std::make_unique<EnemyFactory>(manager, enemies_json);
  m_levels_json = levels_json;
  m_current_level =
    std::make_unique<Level>(m_levels_json[m_current_level_index], *m_manager);
}

LevelManager::LevelManager(std::string_view levels_file, my_assets &manager)
  : m_manager(&manager)
{
  auto file = std::unique_ptr<std::FILE, decltype(&std::fclose)>(
    std::fopen(levels_file.data(), "r"), &std::fclose);
  my_assert(bool(file), "failed to open levels file");

  // TODO, THIS IS BAD
  const auto enemies_filename = "enemies.json";
  auto enemies_file = std::unique_ptr<std::FILE, decltype(&std::fclose)>(
    std::fopen(enemies_filename, "r"), &std::fclose);
  my_assert(bool(enemies_file), "failed to open enemies file");

  m_enemy_factory = std::make_unique<EnemyFactory>(
    manager, nlohmann::json::parse(enemies_file.get()));
  m_levels_json = nlohmann::json::parse(file.get());

  m_current_level =
    std::make_unique<Level>(m_levels_json[m_current_level_index], manager);
}
void LevelManager::update(ginseng::database &db, float delta)
{
  if (auto spawn = m_current_level->update(delta, m_draw_debug);
      spawn != components::enemy_type::none)
  {
    m_enemy_factory->spawn_enemy(db, spawn, m_current_level->m_enemies_turning_points.front());
  }
}


void LevelManager::next_level()
{
  m_current_level_index += 1;
  fmt::println("Loading level {}", m_current_level_index);
  if (m_current_level_index >= m_levels_json.size())
  {
    throw std::runtime_error("No more levels");
  }
  m_current_level.reset();
  m_current_level = std::make_unique<Level>(
    m_levels_json.at(m_current_level_index), *m_manager);
}
void LevelManager::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  target.draw(m_current_level->m_map, states);
}


LevelManager::~LevelManager() {}
