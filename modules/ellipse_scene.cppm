export module ellipse_scene;
import game;
import std;
import raylib;
import bullets;
import draw_system;
import physics_system;
import target_manager;

export
{
  class ellipse_scene : public scene {
private:
    std::vector<bullets> m_bullet_rings;
    std::vector<bullets>::iterator m_current_ring;

    /*std::vector<target> m_targets;*/

    std::vector<detached_bullet> m_detached_bullets;

    bool m_draw_debug_gui = false;
    std::size_t m_score = 0;

    target_manager m_target_manager;
    draw_system m_draw_system;
    physics_system m_physics_system;

public:
    ellipse_scene();

    void on_start() override;
    void on_update() override;
    void draw_debug_gui() const;

    std::unique_ptr<scene> on_exit() override;
    void on_render() override;
  };
}
