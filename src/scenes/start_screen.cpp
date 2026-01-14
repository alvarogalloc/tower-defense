module scenes;
import :shooting;
import wey;
import gui;
import raylib;
using namespace rooster;
inline constexpr int font_size = 48;
inline constexpr Color base_color{.r = colors::darkpurple.r,
                                  .g = colors::darkpurple.g,
                                  .b = colors::darkpurple.b,
                                  .a = 100};

constexpr auto TITLE_FONT = "assets/monogram.ttf";
constexpr auto BLUE_GUY_TEXTURE = "assets/blueguy.png";
constexpr auto MENU_MUSIC = "assets/bgmusic.ogg";
inline constexpr float text_space = 0.2f;
namespace scenes {

start_screen::start_screen(context_view view)
    : state(view), m_space_background("assets/space_config.json", view) {}
struct start_screen_json_data {
  std::string title;
  std::string subtitle;
  std::string text_key;
  int font_size = 0;
};
void start_screen::on_start() {
  m_blue_guy = ctx.assets.get<asset_type::texture>(BLUE_GUY_TEXTURE);
  m_music = ctx.assets.get<asset_type::music>(MENU_MUSIC);
  m_title_font = ctx.assets.get<asset_type::font>(TITLE_FONT, font_size);
  PlayMusicStream(m_music);
  GuiSetFont(m_title_font);
  // build gui
  static const auto json_cfg =
      ctx.assets.get_config<start_screen_json_data>("assets/start_screen.json");

  gui_root.add_child(gui::v2::label{
      json_cfg.title, rooster::colors::yellow, font_size, {10, 10}});

  gui_root.add_child(gui::v2::label{
      json_cfg.subtitle, rooster::colors::yellow, 18, {0, 0}});
  const Vector2 separation{20, 20};
  gui_root.position = separation;
}

void start_screen::on_update() {
  m_space_background.update();
  // if any key pressed
  if (IsKeyReleased(KEY_SPACE)) {
    m_should_exit = true;
  }
  UpdateMusicStream(m_music);
}

std::unique_ptr<state> start_screen::on_exit() {
  UnloadTexture(m_blue_guy);
  // UnloadFont(m_title_font); font is shared with raygui so we don't unload
  // it
  return std::make_unique<scenes::shooting>(ctx);
}

void start_screen::draw_blending_text() {
  const auto start_text{"Press <space> to play"};
  struct blending_effect {
    float speed;
    float min;
    float max;
  };
  const blending_effect blending{.speed = 2.f, .min = 0.2f, .max = 1.f};
  const auto blendingAlpha = [&blending](float time) -> std::uint8_t {
    const float t = (std::sin(time * blending.speed) + 1.0f) * 0.5f;
    const auto interpolated_alpha = static_cast<std::uint8_t>(
        (blending.min + t * (blending.max - blending.min)) * 255);
    return interpolated_alpha;
  };
  auto color = rooster::colors::yellow;
  color.a = blendingAlpha(float(GetTime()));

  const Vector2 text_position{20, 100};
  DrawTextEx(m_title_font, start_text, text_position, font_size, text_space,
             color);
}
void start_screen::on_render() {
  ClearBackground(colors::darkblue);
  m_space_background.draw();
  draw_blending_text();
  gui_root.draw();
}
}  // namespace scenes
