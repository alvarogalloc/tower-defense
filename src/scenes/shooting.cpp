module scenes.shooting;
import raylib;
import systems.player;
import systems.bullet;
import systems.enemy;
import components.movement;
import components.enemy;
import components.tags;

namespace scenes {
  using namespace systems;

  void shooting::on_start()
  {
    m_systems.emplace_back(player::update);
    m_systems.emplace_back(bullet::update);
    m_systems.emplace_back(enemy::update);

    auto new_player = m_world->create_entity();

    m_world->add_component(
      new_player,
      components::movement {
        .position = {400.f, 300.f},
        .velocity = {0.f, 0.f},
        .acceleration_rate = 100.f,
        .deceleration_rate = 15.f,
        .max_speed = 500.f,
        .rotation = 0.f,
      });
    m_world->add_component(
      new_player,
      systems::player::gun {
        .reload_time = 0.5f,
        .last_shot = 0.f,
        .ammo = 50,
        .max_ammo = 50,
      });
    m_world->add_component(new_player, systems::player::action::none);
    m_world->add_component(new_player, components::tags::player {});
    auto spaceship = LoadTexture(SRC_DIR "/assets/spaceship.png");
    m_to_clean.push_back(spaceship);
    m_world->add_component(new_player, spaceship);

    // enemy type 1
    m_enemy_spawner = systems::enemy::make_spawner(
      components::bounding_box {0, 0, 50, 50},
      components::enemy {
        .enemy_type = components::enemy::type::basic,
        .health = 100,
        .max_health = 100,
        .damage = 10,
        .texture = LoadTexture(SRC_DIR "/assets/blueguy.png"),
        .speed = 100.f,
      });
    m_enemy_spawner(*m_world, 0.f, 0.f);
    m_enemy_spawner(*m_world, 150.f, 0.f);
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
    systems::enemy::draw(*m_world);
  }
} // namespace scenes
