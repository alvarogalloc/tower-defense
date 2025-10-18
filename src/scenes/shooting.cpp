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
  m_systems.emplace_back(player::update);
  // m_systems.emplace_back(bullet::update);
  m_systems.emplace_back(enemy::update);

  auto &world = game::get().get_world();

  player_cfg cfg;
  std::string buffer;
  auto ec = glz::read_file_json(cfg, SRC_DIR "/assets/player.json", buffer);
  debug::my_assert(!ec, std::format("Failed to read player config: {}",
                                    glz::format_error(ec, buffer)));

  world.add_component(m_player_entity, cfg.movement);
  world.add_component(m_player_entity, cfg.gun);
  world.add_component(m_player_entity, cfg.gun);
  world.add_component(m_player_entity, systems::player::action::none);
  world.add_component(m_player_entity, components::misc::player{});
  world.add_component(m_player_entity, cfg.health);
  auto spaceship =
      LoadTexture(std::format("{}/{}", SRC_DIR, cfg.texture).c_str());
  auto shoot_sfx =
      LoadSound(std::format("{}/{}", SRC_DIR, cfg.gun.shoot_sfx_path).c_str());
  const float shoot_volume = 0.5f;
  SetSoundVolume(shoot_sfx, shoot_volume);
  m_to_clean.emplace_back(spaceship);
  m_to_clean.emplace_back(shoot_sfx);
  world.add_component(m_player_entity, spaceship);
  world.add_component(m_player_entity, shoot_sfx);

  std::vector<systems::enemy::spawner_cfg> enemies;
  buffer = "";
  ec = glz::read_file_json(enemies, SRC_DIR "/assets/enemies.json", buffer);
  debug::my_assert(!ec, std::format("Failed to read enemies config: {}",
                                    glz::format_error(ec, buffer)));
  for (const auto &e : enemies) {
    m_enemy_spawners.push_back(systems::enemy::make_spawner(e));
  }
  std::vector<systems::waves::level_cfg> levels;
  buffer = "";
  ec = glz::read_file_json(levels, SRC_DIR "/assets/levels.json", buffer);
  debug::my_assert(!ec, std::format("Failed to read levels config: {}",
                                    glz::format_error(ec, buffer)));

  // m_enemy_spawner = systems::enemy::make_spawner(enemies[0]);
  // m_enemy_spawner(world, 0.f, 120.f);
  // m_enemy_spawner(world, 150.f, 30.f);
}
void shooting::on_update() {
  float dt = GetFrameTime();

  auto &world = game::get().get_world();
  for (auto &system : m_systems) {
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
  for (auto res : m_to_clean) {
    std::visit(
        [](auto &t) {
          if constexpr (std::is_same_v<decltype(t), Texture2D>) {
            UnloadTexture(t);
          } else if constexpr (std::is_same_v<decltype(t), Sound>) {
            UnloadSound(t);
          }
        },
        res);
  }
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
