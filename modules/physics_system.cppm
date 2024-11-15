export module physics_system;

import std;
import raylib;
import bullets;
import target_manager;
import raylib_utils;

export
{
  class physics_system {
public:
    constexpr physics_system(
      target_manager& target_manager, std::vector<detached_bullet>& detached_bullets)
        : m_target_manager(target_manager)
        , m_detached_bullets(detached_bullets)
    {}

    constexpr void update(float dt)
    {
      move_detached_bullets(dt);
      poll_bullet_with_target();
    }

private:
    constexpr void move_detached_bullets(float dt)
    {
      float rand_factor = float(GetRandomValue(-1, 1));
      for (auto& current_bullet : m_detached_bullets) {
        if (!current_bullet.valid()) continue; // Skip invalid bullets

        // Calculate direction to target
        // if no target pos is equal to current_bullet.target_position
        // change to the next
        /*current_bullet.target_position = m_target_manager.closest_to(current_bullet.position).pos;*/
        if (
          std::find_if(
            m_target_manager.get_targets().begin(), m_target_manager.get_targets().end(),
            [&](const auto& target) {
              return target.pos.x == current_bullet.target_position.x
                && target.pos.y == current_bullet.target_position.y;
            })
          == m_target_manager.get_targets().end()) {
          current_bullet.target_position = m_target_manager.closest_to(current_bullet.position).pos;
        }
#if 1
        auto direction = current_bullet.target_position - current_bullet.position;
        auto distance_to_target = Vector2Length(direction);

        auto direction_normalized = Vector2Normalize(direction);

        constexpr static float max_speed = 300.f;
        constexpr static float acceleration = 400.f; // Adjust acceleration value as needed

        // Calculate new velocity with acceleration, but limit acceleration when close to target
        constexpr static float min_acceleration_factor = 0.5f;
        const float acceleration_factor
          = std::clamp(distance_to_target / (max_speed / 2), min_acceleration_factor, 1.0f);

        current_bullet.velocity = current_bullet.velocity+ Vector2{rand_factor, rand_factor}
          + direction_normalized * acceleration * acceleration_factor * dt;

        // Limit velocity to max speed
        if (std::hypot(current_bullet.velocity.x, current_bullet.velocity.y) > max_speed) {
          current_bullet.velocity = Vector2Normalize(current_bullet.velocity) * max_speed;
        }

        // Update position
        current_bullet.position = current_bullet.position + current_bullet.velocity * dt;
#else
        auto direction = current_bullet.target_position - current_bullet.position;
        auto direction_normalized = Vector2Normalize(direction); // Normalize direction vector

        constexpr static float max_speed = 300.f;
        constexpr static float acceleration = 50.f; // Adjust acceleration value as needed

        // Calculate new velocity with acceleration
        current_bullet.velocity
          = current_bullet.velocity + direction_normalized * acceleration * dt;

        // Limit velocity to max speed
        if (std::hypot(current_bullet.velocity.x, current_bullet.velocity.y) > max_speed) {
          current_bullet.velocity = Vector2Normalize(current_bullet.velocity) * max_speed;
        }

        // Update position
        current_bullet.position = current_bullet.position + current_bullet.velocity * dt;
#endif
      }
    }
    constexpr void poll_bullet_with_target()
    {
      std::vector<target>& targets = m_target_manager.get_targets();
      for (auto& target : targets) {
        auto [begin, end] = std::ranges::remove_if(m_detached_bullets, [&](auto& bullet) {
          if (!bullet.valid()) return true;
          return std::hypot(target.pos.x - bullet.position.x, target.pos.y - bullet.position.y)
            < target.radius;
        });
        for (auto it = begin; it != end; ++it) {
          target.health -= it->info->damage;
        }
        m_detached_bullets.erase(begin, end);
      }
    }

    target_manager& m_target_manager;
    std::vector<detached_bullet>& m_detached_bullets;
  };
}
