module scenes.shooting;
import raylib;
import systems.player;
import systems.bullet;
import systems.enemy;
import components.movement;
import components.enemy;
import components.tags;
import systems.camera;
import asset_routes;
import debug;

using namespace rooster;

namespace scenes
{
using debug::my_assert;

using namespace systems;

shooting::shooting()
    : m_bg(200),
      m_camera(systems::camera::make_camera()),
      m_player_entity(game::get().get_world().create_entity()),
      m_stats{.won = false, .score = 0, .time_seconds = int(GetTime())}
{
}
void shooting::on_start()
{
    m_systems.emplace_back(player::update);
    m_systems.emplace_back(bullet::update);
    m_systems.emplace_back(enemy::update);

    auto &world = game::get().get_world();

    world.add_component(m_player_entity, components::movement{
                                             .position = {400.f, 300.f},
                                             .velocity = {0.f, 0.f},
                                             .acceleration_rate = 100.f,
                                             .deceleration_rate = 15.f,
                                             .max_speed = 500.f,
                                             .rotation = 0.f,
                                         });
    world.add_component(
        m_player_entity,
        systems::player::gun{.reload_time = 0.5f,
                             .last_shot = 0.f,
                             .ammo = 50,
                             .max_ammo = 50,
                             .shoot_sfx = LoadSound(routes::SHOOT_SFX)});
    world.add_component(m_player_entity, systems::player::action::none);
    world.add_component(m_player_entity, components::tags::player{});
    world.add_component(m_player_entity, systems::player::health{30});
    auto spaceship = LoadTexture(SRC_DIR "/assets/spaceship.png");
    m_to_clean.push_back(spaceship);
    world.add_component(m_player_entity, spaceship);

    // enemy type 1
    m_enemy_spawner = systems::enemy::make_spawner(
        components::bounding_box{0, 0, 50, 50},
        components::enemy{
            .enemy_type = components::enemy::type::basic,
            .health = 100,
            .max_health = 100,
            .damage = 10,
            .texture = LoadTexture(SRC_DIR "/assets/blueguy.png"),
            .speed = 100.f,
        });
    m_enemy_spawner(world, 0.f, 0.f);
    m_enemy_spawner(world, 150.f, 0.f);
}
void shooting::on_update()
{
    float dt = GetFrameTime();

    auto &world = game::get().get_world();
    for (auto &system : m_systems)
    {
        system(world, dt);
    }
    m_bg.update(dt);

    auto player_health = [&world] {
        std::optional<systems::player::health> player_h;
        world.visit([&player_h](components::tags::player,
                                systems::player::health h) { player_h = h; });
        if (!player_h)
        {
            my_assert(false, "player health not found");
        }
        return *player_h;
    }();
    if (player_health.current <= 0)
    {
        m_should_exit = true;
        m_stats.won = false;
        m_stats.score = 0; // TODO: calculate score
    }
}

std::unique_ptr<scene> shooting::on_exit()
{
    for (auto texture : m_to_clean)
    {
        UnloadTexture(texture);
    }
    m_stats.time_seconds = int(GetTime() - m_stats.time_seconds);
    return std::make_unique<scenes::gameover>(m_stats);
}
void shooting::on_render()
{
    systems::camera::use(
        m_camera, game::get().get_world(), m_player_entity, [this]() {
            constexpr static auto bgcolor = Color{5, 15, 45, 255};
            ClearBackground(bgcolor);
            m_bg.draw();
            auto &world = game::get().get_world();
            systems::player::draw(world);
            systems::bullet::draw(world);
            systems::enemy::draw(world);
        });
}
} // namespace scenes
