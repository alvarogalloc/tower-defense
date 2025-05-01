export module scenes.menu;
import game;
import raylib;
import std;
import prefabs.space_background;

export namespace scenes
{
class menu : public scene
{
    Font m_title_font;
    bool m_should_exit_game = false;
    Texture2D m_blue_guy;
    prefabs::v2::space_background m_space_background;
    Music m_music;

  public:
    menu(game &game)
        : scene(game),
          m_space_background(
              SRC_DIR "/assets/title_bg/blue-back.png",
              std::vector<std::string>{SRC_DIR "/assets/title_bg/prop-planet-big.png", SRC_DIR "/assets/title_bg/prop-planet-small.png"}, 0.5f)
    {
    }
    [[nodiscard]] bool should_exit_game() const override
    {
        return m_should_exit_game;
    }
    void on_start() override;
    void on_update() override;

    std::unique_ptr<scene> on_exit() override;
    void on_render() override;
};
} // namespace scenes
