module scenes.shooting;
import raylib;
import systems.player;
import systems.bullet;
import systems.waves;
import systems.enemy;
import glaze;
import components.movement;
import components.enemy;
import raylib_utils;
import components.misc;
import systems.camera;
import asset_routes;
import debug;
import system_manager;
import resource_manager;
import event_system;

using namespace rooster;

static debug::message *camera_msg{};

namespace scenes {
using debug::my_assert;

using namespace systems;
constexpr int n_particles = 150;

struct player_cfg {
  components::movement movement;
  systems::player::gun gun;
  systems::player::health health;
  std::string texture;
};

shooting::shooting()
    : m_particles(n_particles),
      m_camera(systems::camera::make_camera()),
      m_player_entity(game::get().get_world().create_entity()),
      m_gun_shoot_sfx{},
      m_stats{
          .won = false,
          .score = 0,
          .time_seconds = int(GetTime()),
      } {}

void shooting::on_start() {
  camera_msg =
      &game::get().push_debug_message({.lifetime_seconds = -1.f, .text = ""});
  
  // Register systems with priorities (lower numbers run first)
  m_system_mgr.register_system("player", 
      [this](ginseng::database& db, float dt, systems::Phase phase) {
          systems::player::system(db, dt, phase, &m_event_bus);
      }, 10);
  
  m_system_mgr.register_system("enemy", 
      [](ginseng::database& db, float dt, systems::Phase phase) {
          systems::enemy::system(db, dt, phase);
      }, 20);
  
  // Legacy systems (will be migrated in future)
  m_legacy_systems.emplace_back(player::update);
  m_legacy_systems.emplace_back(enemy::update);

  auto &world = game::get().get_world();

  const auto spawn_player = [&] {
    std::string buffer;
    player_cfg cfg;
    const auto ec =
        glz::read_file_json(cfg, SRC_DIR "/assets/player.json", buffer);
    debug::my_assert(!ec, std::format("Failed to read player config: {}",
                                      glz::format_error(ec, buffer)));

    world.add_component(m_player_entity, cfg.movement);
    world.add_component(m_player_entity, cfg.gun);
    world.add_component(m_player_entity, systems::player::action::none);
    world.add_component(m_player_entity, components::misc::player{});
    world.add_component(m_player_entity, cfg.health);
    
    // Use resource manager for automatic cleanup
    const auto spaceship = m_resources.load_texture(
        std::format("{}/{}", SRC_DIR, cfg.texture).c_str());
    const auto shoot_sfx = m_resources.load_sound(
        std::format("{}/{}", SRC_DIR, cfg.gun.shoot_sfx_path).c_str());
    const float shoot_volume = 0.5f;
    SetSoundVolume(shoot_sfx, shoot_volume);
    
    world.add_component(m_player_entity, spaceship);
    world.add_component(m_player_entity, shoot_sfx);
  };
  const auto load_enemies = [&] {
    std::vector<systems::enemy::spawner_cfg> enemies;
    std::string buffer;
    const auto ec =
        glz::read_file_json(enemies, SRC_DIR "/assets/enemies.json", buffer);
    debug::my_assert(!ec, std::format("Failed to read enemies config: {}",
                                      glz::format_error(ec, buffer)));
    for (const auto &e : enemies) {
      m_enemy_spawners.push_back(systems::enemy::make_spawner(e));
    }
  };
  const auto load_levels = [&] {
    std::string buffer;
    const auto ec =
        glz::read_file_json(m_levels, SRC_DIR "/assets/levels.json", buffer);
    debug::my_assert(!ec, std::format("Failed to read levels config: {}",
                                      glz::format_error(ec, buffer)));
    my_assert(m_levels.size(), "there should be at least one level");
  };

  spawn_player();
  load_enemies();
  load_levels();
  m_legacy_systems.emplace_back(systems::waves::make_level_update(m_levels, m_enemy_spawners));
  
  // Initialize all systems
  m_system_mgr.execute(world, 0.0f, systems::Phase::Init);
}
void shooting::on_update() {
  float dt = GetFrameTime();

  auto &world = game::get().get_world();
  
  // Execute new architecture systems (commented out to maintain backward compatibility)
  // m_system_mgr.execute(world, dt, systems::Phase::Update);
  
  // Execute legacy systems for now
  for (auto &system : m_legacy_systems) {
    system(world, dt);
  }
  
  const auto cam = Rectangle{
      .x = m_camera.target.x - m_camera.offset.x,
      .y = m_camera.target.y - m_camera.offset.y,
      .width = m_camera.offset.x * 2,
      .height = m_camera.offset.y * 2,
  };
  systems::bullet::update(world, dt, cam);
  m_particles.update(dt, cam);
  camera_msg->text = std::format("camera {}", to_string(cam));

  auto player_health = [&world] {
    std::optional<systems::player::health> player_h;
    world.visit([&player_h](components::misc::player,
                            systems::player::health h) { player_h = h; });
    if (!player_h) {
      my_assert(false, "player health not found");
    }
    return *player_h;
  }();
  if (player_health.current <= 0) {
    m_should_exit = true;
    m_stats.won = false;
    m_stats.score = 0;  // TODO: calculate score
  }
}

std::unique_ptr<scene> shooting::on_exit() {
  // Cleanup systems
  auto &world = game::get().get_world();
  m_system_mgr.execute(world, 0.0f, systems::Phase::Cleanup);
  
  // Resources are automatically cleaned up by ResourceManager destructor
  
  m_stats.time_seconds = int(GetTime() - m_stats.time_seconds);
  return std::make_unique<scenes::gameover>(m_stats);
}
void shooting::on_render() {
  systems::camera::use(m_camera, game::get().get_world(), m_player_entity,
                       [this]() {
                         constexpr static auto bgcolor = Color{5, 15, 45, 255};
                         ClearBackground(bgcolor);
                         m_particles.draw();
                         auto &world = game::get().get_world();
                         systems::player::draw(world);
                         systems::bullet::draw(world);
                         systems::enemy::draw(world);
                       });
}
}  // namespace scenes
