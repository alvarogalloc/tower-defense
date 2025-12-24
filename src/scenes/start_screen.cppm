export module scenes.start_screen;
import state;
import raylib;
import std;
import prefabs.space_background;

export namespace scenes {
class start_screen : public state {
  Font m_title_font{};
  Texture2D m_blue_guy{};
  prefabs::space_background m_space_background;
  Music m_music{};
  bool m_should_exit{false};

 public:
  start_screen(context_view);
  void on_start() override;
  void on_update() override;
  [[nodiscard]] bool should_exit() const override { return m_should_exit; }

  std::unique_ptr<state> on_exit() override;
  void on_render()  override;
};

}  // namespace scenes
