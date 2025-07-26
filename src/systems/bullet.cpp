module systems.bullet;
import config;
import components.enemy;
import components.movement;
using namespace rooster;

namespace systems::bullet
{
void shoot_bullet(ginseng::database &db, components::bullet info)
{
    auto new_bullet = db.create_entity();
    db.add_component(new_bullet, std::move(info));
}

void update(ginseng::database &db, float dt)
{
    const auto is_out_of_bounds = [&](const components::bullet &bullet) {
        return bullet.position.x < 0 ||
               bullet.position.x > config::game_info.size.x ||
               bullet.position.y < 0 ||
               bullet.position.y > config::game_info.size.y;
    };
    db.visit([&](ginseng::database::ent_id bullet_id,
                 components::bullet &bullet) {
        const auto sin_value = std::sin(bullet.rotation - 3.14f / 2.f);
        const auto cos_value = std::cos(bullet.rotation - 3.14f / 2.f);
        const float bullet_speed = 850.0f;
        bullet.velocity = Vector2Scale({cos_value, sin_value}, bullet_speed);
        bullet.position =
            Vector2Add(bullet.position, Vector2Scale(bullet.velocity, dt));
        if (is_out_of_bounds(bullet))
        {
            db.destroy_entity(bullet_id);
            return;
        }

        // now collision with enemies
        db.visit([&](ginseng::database::ent_id enemy_id,
                     components::enemy &enemy, components::bounding_box &box) {
            if (CheckCollisionCircleRec(bullet.position, bullet.radius, box))
            {
                enemy.health -= bullet.damage;
                db.destroy_entity(bullet_id);
                if (enemy.health <= 0)
                {
                    db.destroy_entity(enemy_id);
                }
            }
        });
    });
}

void draw(ginseng::database &db)
{
    db.visit([&](const components::bullet &bullet) {
        DrawCircleV(bullet.position, bullet.radius, colors::white);
    });
}

} // namespace systems::bullet
