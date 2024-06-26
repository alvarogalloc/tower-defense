export module BulletSystem;

import stdbridge;
import ginseng;
import utils;
import components;
import sfml;

export std::function<void(float)>
  make_bullet_system(ginseng::database &db, const sf::Vector2f &win_size)
{
  return [&](float delta) {
    db.visit(
      [&](ginseng::database::ent_id id, components::Projectile &projectile) {
        if (projectile.getPosition().x < 0
            || projectile.getPosition().x > win_size.x
            || projectile.getPosition().y < 0
            || projectile.getPosition().y > win_size.y)
        {
          db.destroy_entity(id);
        } else
        {
          projectile.move(projectile.direction * projectile.speed * delta);
          projectile.rotate(delta * 1000);

          db.visit(
            [&](components::Animation &anim, components::Health &health) {
              if (db.exists(id)
                  && projectile.getGlobalBounds().intersects(
                    anim.getGlobalBounds()))
              {
                health.health -= projectile.damage;
                db.destroy_entity(id);
              }
            });
        }
      });
  };
}
