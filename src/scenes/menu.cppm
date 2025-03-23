export module scenes.menu;
import game;
import raylib;
import std;
import prefabs.space_background;

export namespace scenes
{
  class menu : public scene {
    Font m_title_font;
    Texture2D m_blue_guy;
    bool m_should_exit_game = false;
    prefabs::space_background m_space_background;

public:
    bool should_exit_game() const override
    {
      return m_should_exit_game;
    }
    void on_start() override;
    void on_update() override;

    std::unique_ptr<scene> on_exit() override;
    void on_render() override;
  };
}
