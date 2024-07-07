export module BulletSystem;

import stdbridge;
import ginseng;
import utils;
import components;
import sfml;

export void spawn_bullet(ginseng::database &db,
  sf::Vector2f startpos,
  sf::Vector2f destinypos,
  components::ProjectileDefinition def)
{
  auto bullet_id = db.create_entity();
  components::Projectile projectile;
  // calculate the direction of the bullet with mouse
  // position
  projectile.direction = normalize(destinypos - startpos);
  projectile.speed = def.speed;
  projectile.damage = def.damage;
  projectile.setSize({ 5, 5 });
  // rotate the bullet by a fixed amount
  projectile.setPosition(startpos);
  projectile.setOrigin(sprite_center(projectile));
  db.add_component(bullet_id, projectile);
}

export std::function<void(float)> make_bullet_system(ginseng::database &db,
  const sf::Vector2f &win_size)
{
  return [&](float delta) {
    db.visit([&](ginseng::database::ent_id id,
               components::Projectile &projectile) {
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

        db.visit([&](components::Animation &anim, components::Health &health) {
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
