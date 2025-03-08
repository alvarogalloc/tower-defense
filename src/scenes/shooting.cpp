module scenes.shooting;
import raylib;
import systems.player;
import systems.bullet;
import systems.enemy;
import components.movement;

namespace scenes {
  shooting::shooting() {}

  void shooting::on_start()
  {
    m_systems.push_back(systems::player::update);
    m_systems.push_back(systems::bullet::update);
    m_systems.push_back(systems::enemy::update);

    auto new_player = m_world->create_entity();

    m_world->add_component(
      new_player,
      components::movement {
        .position = {400.f, 300.f},
        .velocity = {0.f, 0.f},
        .acceleration_rate = 100.f,
        .deceleration_rate = 10.f,
        .max_speed = 200.f,
        .rotation = 0.f,
      });
    m_world->add_component(
      new_player,
      systems::player::gun {
        .reload_time = 0.5f,
        .last_shot = 0.f,
        .ammo = 10,
        .max_ammo = 10,
      });
    m_world->add_component(new_player, systems::player::action::none);
    auto spaceship = LoadTexture(SRC_DIR "/assets/spaceship.png");
    m_to_clean.push_back(spaceship);
    m_world->add_component(new_player, spaceship);
  }
  void shooting::on_update()
  {
    float dt = GetFrameTime();
    for (auto& system : m_systems) {
      system(*m_world, dt);
    }
  }

  std::unique_ptr<scene> shooting::on_exit()
  {
    for (auto texture : m_to_clean) {
      UnloadTexture(texture);
    }
    return nullptr;
  }
  void shooting::on_render()
  {
    ClearBackground(colors::darkblue);
    systems::player::draw(*m_world);
    systems::bullet::draw(*m_world);
  }
} // namespace scenes
