module systems.bullet;
import config;

namespace systems::bullet {
  void shoot_bullet(ginseng::database& db, info info)
  {
    auto new_bullet = db.create_entity();
    db.add_component(new_bullet, std::move(info));
  }

  void update(ginseng::database& db, float dt)
  {
    const auto is_out_of_bounds = [&](const info& bullet) {
      return bullet.position.x < 0 || bullet.position.x > config::game_info.size.x || bullet.position.y < 0
        || bullet.position.y > config::game_info.size.y;
    };
    db.visit([&](ginseng::database::ent_id id, info& bullet) {
      const auto sin_value = std::sin(bullet.rotation - 3.14f / 2.f);
      const auto cos_value = std::cos(bullet.rotation - 3.14f / 2.f);
      const float bullet_speed = 850.0f;
      bullet.velocity = Vector2Scale({cos_value, sin_value}, bullet_speed);
      bullet.position = Vector2Add(bullet.position, Vector2Scale(bullet.velocity, dt));
      if (is_out_of_bounds(bullet)) {
        db.destroy_entity(id);
      }
    });
  }

  void draw(ginseng::database& db)
  {
    db.visit([&](const info& bullet) {
      DrawCircleV(bullet.position, 5.f, colors::white);
    });
  }

} // namespace systems::bullet
