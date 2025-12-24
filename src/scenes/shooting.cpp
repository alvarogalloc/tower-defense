module scenes.shooting;
import raylib;
import systems.player;
import systems.bullet;
import systems.waves;
import systems.enemy;
import game;
import glaze;
import components.movement;
import components.enemy;
import components.misc;
import systems.camera;
import debug;

using namespace rooster;


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

shooting::shooting(context_view view)
    : state(view),
      m_particles(n_particles, view),
      m_camera(),
      m_player_entity(view.ecs.create()),
      m_gun_shoot_sfx{},
      m_stats{
          .won = false,
          .score = 0,
          .time_seconds = int(GetTime()),
      } {}

void shooting::on_start() {
  m_camera = systems::camera::make_camera(ctx.app_info.game_res);
  m_systems.emplace_back(player::update);
  // m_systems.emplace_back(bullet::update);
  m_systems.emplace_back(enemy::update);

  auto &world = ctx.ecs;

  const auto spawn_player = [&] {
    auto cfg = ctx.assets.get_config<player_cfg>("assets/player.json");
    /// use assets::get_config

    world.emplace<components::movement>(m_player_entity, cfg.movement);
    world.emplace<systems::player::gun>(m_player_entity, cfg.gun);
    world.emplace<systems::player::action>(m_player_entity,
                                           systems::player::action::none);
    world.emplace<components::misc::player>(m_player_entity);
    world.emplace<systems::player::health>(m_player_entity, cfg.health);
    const auto spaceship = ctx.assets.get<asset_type::texture>(cfg.texture);
    const auto shoot_sfx =
        ctx.assets.get<asset_type::sfx>(cfg.gun.shoot_sfx_path);
    const float shoot_volume = 0.5f;
    SetSoundVolume(shoot_sfx, shoot_volume);
    m_to_clean.emplace_back(spaceship);
    m_to_clean.emplace_back(shoot_sfx);
    world.emplace<Texture2D>(m_player_entity, spaceship);
    world.emplace<Sound>(m_player_entity, shoot_sfx);
  };
  const auto load_enemies = [&] {
    auto enemies =
        ctx.assets.get_config<std::vector<systems::enemy::spawner_cfg>>(
            "assets/enemies.json");
    for (const auto &e : enemies) {
      m_enemy_spawners.push_back(systems::enemy::make_spawner(e, ctx));
    }
  };

  spawn_player();
  load_enemies();

  m_levels = ctx.assets.get_config<std::vector<systems::waves::level_cfg>>(
      "assets/levels.json");
  my_assert(m_levels.size(), "there should be at least one level");

  m_systems.emplace_back(
      systems::waves::make_level_update(m_levels, m_enemy_spawners));
}
void shooting::on_update() {
  float dt = GetFrameTime();
  // static float time_for_next_spawn = 0.f;

  auto &world = ctx.ecs;
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

  auto player_health = world.get<systems::player::health>(m_player_entity);

  if (player_health.current <= 0) {
    m_should_exit = true;
    m_stats.won = false;
    m_stats.score = 0;  // TODO: calculate score
  }
}

std::unique_ptr<state> shooting::on_exit() {
  debug::my_assert(m_should_exit,
                   "we did not exit normally from the shooting scene");
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
  return std::make_unique<scenes::gameover>(
      context_view{
          .ecs = ctx.ecs,
          .app_info = ctx.app_info,
          .assets = ctx.assets,
      },
      m_stats);
}
void shooting::on_render() {
  systems::camera::use(m_camera, ctx.ecs, m_player_entity, [this]() {
    constexpr static auto bgcolor = Color{5, 15, 45, 255};
    ClearBackground(bgcolor);
    m_particles.draw();
    auto &world = ctx.ecs;
    systems::player::draw(world);
    systems::bullet::draw(world);
    systems::enemy::draw(world);
  });
}
}  // namespace scenes
