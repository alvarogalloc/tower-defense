export module scenes:start_screen;
import wey;
import raylib;
import std;
import prefabs;
import gui;
import glaze;

struct start_screen_json_data {
  std::string title;
  std::string subtitle;
  std::string text_key;
  int title_font_size = 0;
  int subtitle_font_size = 0;
  Color text_color = rooster::colors::yellow;
  Color bg_color = rooster::colors::darkblue;
};
template <>
struct glz::meta<start_screen_json_data> {
  using T = start_screen_json_data;
  static constexpr auto value = glz::object(
      "title", &T::title, "subtitle", &T::subtitle, "text_key", &T::text_key,
      "title_font_size", &T::title_font_size, "subtitle_font_size",
      &T::subtitle_font_size, "text_color", glz::as_array<&T::text_color>,
      "bg_color", glz::as_array<&T::bg_color>);
};

export namespace scenes {

class start_screen : public state {
  Font m_title_font{};
  Texture2D m_blue_guy{};
  prefabs::space_background m_space_background;
  Music m_music{};
  bool m_should_exit{false};
  gui::v2::container gui_root;
  start_screen_json_data json_cfg;


 public:
  start_screen(context_view);
  void on_start() override;
  void on_update() override;
  [[nodiscard]] bool should_exit() const override { return m_should_exit; }

  std::unique_ptr<state> on_exit() override;
  void on_render() override;

 private:
  void draw_blending_text();
};

}  // namespace scenes
