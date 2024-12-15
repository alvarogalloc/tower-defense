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
      for (auto& bullet : m_detached_bullets) {
        if (!bullet.valid()) continue;
        bullet.chase.update(dt);
        bullet.position = bullet.chase.get_current_position();
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
