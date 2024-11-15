export module bullets;
import std;
import raylib;
import fmt;
import raylib_utils;
export
{
  struct bullet {
    bool alive = true;
    float acc_time = 0.f;
  };

  struct bullet_group_info {
    float damage = 0.f;
    float radius = 0.f;
    float speed = 0.f;
    Color color = colors::red;
    std::size_t count = 0;
    Vector2 ellipse_radius;
    // deserialize it from file
    static bullet_group_info load(std::string_view filename);
  };
  struct detached_bullet {
    bullet_group_info* info {nullptr};
    Vector2 target_position {0, 0};
    Vector2 position {0, 0};
    Vector2 velocity {0, 0};
    bool valid() const
    {
      return info != nullptr;
    }
  };

  class bullets {
    // this is the class for the bullets suroounding the player
    // it will be used to draw the bullets and update their position
    // and to take track of the bullets that are still alive and their damage

public:
    bullets(const bullet_group_info& info);

    auto get_info() const
    {
      return m_info;
    }

    detached_bullet detach_bullet(const Vector2 center);
    Vector2 get_edge_for(const Vector2 target, const Vector2 center) const;
    // position of the player should be provided
    void draw(const Vector2) const;
    // overload to draw the ellipse when that ring is selected
    void draw(const Vector2, bool draw_ellipse) const;

    void respawn_dead();
    void update(const float delta);

    std::vector<bullet>& get()
    {
      return m_bullets;
    }

    auto count_alive() const
    {
      return std::ranges::count_if(m_bullets, [](const auto& b) {
        return b.alive;
      });
    }
    auto count_dead() const
    {
      return std::ranges::count_if(m_bullets, [](const auto& b) {
        return !b.alive;
      });
    }

private:
    std::vector<bullet> m_bullets;
    bullet_group_info m_info;
  };
}
