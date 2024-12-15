export module bullets;
import std;
import raylib;
import fmt;
import raylib_utils;
import debug;
export
{
  class bullet_chase {
    std::vector<Vector2> m_points;
    Vector2 current_position {0, 0};
    float m_speed = 0.f;

public:
    explicit bullet_chase(const std::vector<Vector2>& points, float speed);
    void add_point(const Vector2& point);
    void reset();
    void update(float delta);
    auto get_points() const
    {
      return m_points;
    }

    auto get_current_position() const
    {
      return current_position;
    }
  };

  struct detached_bullet;
  class bullets {
    // this is the class for the bullets suroounding the player
    // it will be used to draw the bullets and update their position
    // and to take track of the bullets that are still alive and their damage

public:
    struct bullet {
      bool alive = true;
      float acc_time = 0.f;
    };
    struct info {
      float damage = 0.f;
      float radius = 0.f;
      float speed = 0.f;
      Color color = colors::red;
      std::size_t count = 0;
      Vector2 ellipse_radius;
      // deserialize it from file
      static info load(std::string_view filename);
    };

    bullets(const info& info);

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
    info m_info;
  };

  struct detached_bullet {
    bullets::info* info {nullptr};
    // MAYBE FIX THIS, NOT WANT INVALID CHASES
    bullet_chase chase {{}, 0.f};
    Vector2 target_position {0, 0};
    Vector2 position {0, 0};
    Vector2 velocity {0, 0};
    bool valid() const
    {
      return info != nullptr;
    }
  };
}
