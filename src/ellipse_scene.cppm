export module ellipse_scene;
import game;
import std;
import raylib;
import draw_system;
import target_manager;
import player_system;

export
{
  class ellipse_scene : public scene {
private:
    std::size_t m_score = 0;
    target_manager m_target_manager;
    player_movement m_player_movement;
    std::function<void(float)> m_update_player;

    Texture2D m_spaceship;


public:
    ellipse_scene();

    void on_start() override;
    void on_update() override;

    std::unique_ptr<scene> on_exit() override;
    void on_render() override;
  };
}
