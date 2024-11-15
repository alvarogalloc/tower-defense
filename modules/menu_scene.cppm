export module menu_scene;
import game;
import raylib;
import std;

export
{
  class menu_scene : public scene {
    Font m_title_font;
    Texture2D m_blue_guy;

public:
    void on_start() override;
    void on_update() override;

    std::unique_ptr<scene> on_exit() override;
    void on_render() override;
  };
}
