export module ellipse_scene;
import game;
import std;
import raylib;
import bullets;
import draw_system;
import physics_system;
import target_manager;
import debug_gui_system;

export
{
  class ellipse_scene : public scene {
private:
    std::vector<bullets> m_bullet_rings;
    std::size_t m_selected_ring_index = 0;

    std::vector<detached_bullet> m_detached_bullets;

    bool m_draw_debug_gui = false;
    std::size_t m_score = 0;

    target_manager m_target_manager;
    draw_system m_draw_system;
    physics_system m_physics_system;
    debug_gui_system m_debug_gui_system;

public:
    ellipse_scene();

    void on_start() override;
    void on_update() override;

    std::unique_ptr<scene> on_exit() override;
    void on_render() override;
  };
}
