export module ellipse_scene;
import game;
import std;
import raylib;
import bullets;

struct target {
  Vector2 pos;
  float radius;
  Color color;
  float health;
  float max_health;
};

export
{
  class ellipse_scene : public scene {
private:
    std::vector<bullets> m_bullet_rings;
    std::vector<target> m_targets;
    std::vector<target>::iterator m_closest_target;
    detached_bullet m_detached_bullet;

    bool m_draw_debug_gui = false;

public:
    ellipse_scene();
    void on_start() override;
    void on_update() override;
    void draw_debug_gui() const;

    std::unique_ptr<scene> on_exit() override;
    void on_render() override;
  };
}
