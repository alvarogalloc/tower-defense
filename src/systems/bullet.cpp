module systems.bullet;
import config;
import components.enemy;
import components.movement;
import system_manager;
import event_system;
using namespace rooster;

namespace systems::bullet
{
void shoot_bullet(ginseng::database &db, components::bullet info)
{
    auto new_bullet = db.create_entity();
    db.add_component(new_bullet, std::move(info));
}

// New unified system function with phase support
void system(ginseng::database &db, float dt, systems::Phase phase, Rectangle bounds, systems::EventBus* event_bus)
{
    if (phase == systems::Phase::Init) {
        // Subscribe to shoot events
        if (event_bus) {
            event_bus->subscribe<systems::ShootEvent>([&db](const systems::Event& base_event) {
                const auto& event = static_cast<const systems::ShootEvent&>(base_event);
                shoot_bullet(db, components::bullet{
                    .position = event.position,
                    .velocity = event.velocity,
                    .rotation = event.rotation,
                    .damage = static_cast<std::uint8_t>(event.damage),
                    .radius = 4.5f
                });
            });
        }
        return;
    }
    if (phase == systems::Phase::Cleanup) {
        // Nothing to cleanup for bullet system currently
        return;
    }
    // Phase::Update - run the update logic
    update(db, dt, bounds);
}

void update(ginseng::database &db, float dt, Rectangle )
{
    std::vector<ginseng::database::ent_id> entities_to_destroy;
    db.visit([&](ginseng::database::ent_id bullet_id,
                 components::bullet &bullet) {
        const auto sin_value = std::sin(bullet.rotation - 3.14f / 2.f);
        const auto cos_value = std::cos(bullet.rotation - 3.14f / 2.f);
        const float bullet_speed = 850.0f;
        bullet.velocity = Vector2Scale({cos_value, sin_value}, bullet_speed);
        bullet.position =
            Vector2Add(bullet.position, Vector2Scale(bullet.velocity, dt));
        // if (CheckCollisionPointRec(bullet.position, bounds))
        // {
        //     db.destroy_entity(bullet_id);
        //     return;
        // }

        // now collision with enemies
        db.visit([&](ginseng::database::ent_id enemy_id,
                     components::enemy &enemy, components::bounding_box &box) {
            if (CheckCollisionCircleRec(bullet.position, bullet.radius, box))
            {
                enemy.health -= bullet.damage;
                entities_to_destroy.emplace_back(bullet_id);
                if (enemy.health <= 0)
                {
                    entities_to_destroy.emplace_back(enemy_id);
                }
            }
        });
    });
    std::ranges::for_each(entities_to_destroy,
                          [&](auto id) { db.destroy_entity(id); });
}

void draw(ginseng::database &db)
{
    db.visit([&](const components::bullet &bullet) {
        DrawCircleV(bullet.position, bullet.radius, colors::white);
    });
}

} // namespace systems::bullet
