export module ellipse_scene;
import game;
import std;
import raylib;
import bullets;


export
{
  class ellipse_scene : public scene {
private:
    std::vector<bullets> m_bullet_rings;
    std::vector<bullets>::iterator m_current_ring;

    std::vector<target> m_targets;
    std::vector<target>::iterator m_closest_target;

    std::vector<detached_bullet> m_detached_bullets;

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
