module scenes.shooting;
import raylib;
import systems.player;
import systems.bullet;
import systems.waves;
import utils.assets_cache;
import systems.enemy;
import game;
import glaze;
import components.movement;
import components.enemy;
import components.misc;
import systems.camera;
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
      m_player_entity(game::get().get_world().create()),
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

  const auto spawn_player = [&] {
    std::string buffer;
    player_cfg cfg;
    const auto ec = glz::read_file_json(
        cfg, game::get().get_config().get_path("assets/player.json"), buffer);
    debug::my_assert(!ec, std::format("Failed to read player config: {}",
                                      glz::format_error(ec, buffer)));

    world.emplace<components::movement>(m_player_entity, cfg.movement);
    world.emplace<systems::player::gun>(m_player_entity, cfg.gun);
    world.emplace<systems::player::action>(m_player_entity,
                                           systems::player::action::none);
    world.emplace<components::misc::player>(m_player_entity);
    world.emplace<systems::player::health>(m_player_entity, cfg.health);
    const auto spaceship =
        utils::get_asset<utils::asset_type::texture>(cfg.texture);
    const auto shoot_sfx =
        utils::get_asset<utils::asset_type::sfx>(cfg.gun.shoot_sfx_path);
    const float shoot_volume = 0.5f;
    SetSoundVolume(shoot_sfx, shoot_volume);
    m_to_clean.emplace_back(spaceship);
    m_to_clean.emplace_back(shoot_sfx);
    world.emplace<Texture2D>(m_player_entity, spaceship);
    world.emplace<Sound>(m_player_entity, shoot_sfx);
  };
  const auto load_enemies = [&] {
    std::vector<systems::enemy::spawner_cfg> enemies;
    std::string buffer;
    const auto ec = glz::read_file_json(
        enemies, game::get().get_config().get_path("assets/enemies.json"), buffer);
    debug::my_assert(!ec, std::format("Failed to read enemies config: {}",
                                      glz::format_error(ec, buffer)));
    for (const auto &e : enemies) {
      m_enemy_spawners.push_back(systems::enemy::make_spawner(e));
    }
  };
  const auto load_levels = [&] {
    std::string buffer;
    const auto ec = glz::read_file_json(
        m_levels, game::get().get_config().get_path("assets/levels.json"), buffer);
    debug::my_assert(!ec, std::format("Failed to read levels config: {}",
                                      glz::format_error(ec, buffer)));
    my_assert(m_levels.size(), "there should be at least one level");
  };

  spawn_player();
  load_enemies();
  load_levels();
  m_systems.emplace_back(
      systems::waves::make_level_update(m_levels, m_enemy_spawners));
}
void shooting::on_update() {
  float dt = GetFrameTime();
  // static float time_for_next_spawn = 0.f;

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
  camera_msg->text = std::format("camera {}", ::to_string(cam));

  auto player_health = world.get<systems::player::health>(m_player_entity);

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
