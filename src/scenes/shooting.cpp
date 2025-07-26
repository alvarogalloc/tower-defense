module scenes.shooting;
import raylib;
import systems.player;
import systems.bullet;
import systems.enemy;
import components.movement;
import components.enemy;
import components.tags;
import asset_routes;

using namespace rooster;

namespace scenes
{
using namespace systems;

shooting::shooting() : m_bg(200)
{
}
void shooting::on_start()
{
    m_systems.emplace_back(player::update);
    m_systems.emplace_back(bullet::update);
    m_systems.emplace_back(enemy::update);

    auto &world = game::get().get_world();
    auto new_player = world.create_entity();

    world.add_component(new_player, components::movement{
                                        .position = {400.f, 300.f},
                                        .velocity = {0.f, 0.f},
                                        .acceleration_rate = 100.f,
                                        .deceleration_rate = 15.f,
                                        .max_speed = 500.f,
                                        .rotation = 0.f,
                                    });
    world.add_component(new_player, systems::player::gun{
                                        .reload_time = 0.5f,
                                        .last_shot = 0.f,
                                        .ammo = 50,
                                        .max_ammo = 50,
                                        .shoot_sfx= LoadSound(routes::SHOOT_SFX)
                                    });
    world.add_component(new_player, systems::player::action::none);
    world.add_component(new_player, components::tags::player{});
    world.add_component(new_player, systems::player::health{100});
    auto spaceship = LoadTexture(SRC_DIR "/assets/spaceship.png");
    m_to_clean.push_back(spaceship);
    world.add_component(new_player, spaceship);

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
    m_enemy_spawner(world, 50.f, 0.f);
    m_enemy_spawner(world, 140.f, 0.f);
    m_enemy_spawner(world, 110.f, 0.f);
    m_enemy_spawner(world, -140.f, 0.f);
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
}

std::unique_ptr<scene> shooting::on_exit()
{
    for (auto texture : m_to_clean)
    {
        UnloadTexture(texture);
    }
    return nullptr;
}
void shooting::on_render()
{
    constexpr static auto bgcolor = Color{5, 15, 45, 255};
    ClearBackground(bgcolor);
    m_bg.draw();
    auto &world = game::get().get_world();
    systems::player::draw(world);
    systems::bullet::draw(world);
    systems::enemy::draw(world);
}
} // namespace scenes
