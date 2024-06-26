export module EnemySystem;

import stdbridge;
import ginseng;
import utils;
import components;
import sfml;

export
{
  std::function<void(float, const std::vector<sf::Vector2f> &)>
    make_enemy_system(ginseng::database & db)
  {
    return [&db](float delta, const std::vector<sf::Vector2f> &directions) {
      db.visit([&](ginseng::database::ent_id id,
                 components::Animation &anim,
                 components::EnemyPath &path,
                 const components::Velocity &vel,
                 components::Health &health) {
        if (health.health <= 0)
        {
          db.destroy_entity(id);
        } else
        {
          bool should_delete = false;
          // move the anim to the next point

          // get unit vector pointing to the next point
          my_assert(path.current_point < directions.size(),
            "current goal point is out of bounds");
          const auto center = sprite_center(anim);
          float distance = std::hypot(
            directions[path.current_point].x - center.x - anim.getPosition().x,
            directions[path.current_point].y - center.y - anim.getPosition().y);
          // std::sqrt(std::pow(
          //             2)
          //           + std::pow(,
          //             2));
          float min_distance_for_next_point = 3;
          if (distance < min_distance_for_next_point)
          {
            if (path.current_point == directions.size() - 1)
            {
              path.current_point = 0;
              should_delete = true;
            } else
            {
              ++path.current_point;
            }
          }
          auto move_vector = normalize(directions[path.current_point] - center
                                       - anim.getPosition())
                             * vel.magnitude;
          anim.move(move_vector * delta);
          if (should_delete) db.destroy_entity(id);
        }
      });
    };
  }
}
