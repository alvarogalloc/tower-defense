module scenes.start_screen;
import scenes.shooting;
import config;
import gui;
import assets;
using namespace rooster;
inline constexpr int font_size = 20;
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
void start_screen::on_start() {
  m_blue_guy = ctx.assets.get<asset_type::texture>(BLUE_GUY_TEXTURE);
  m_music = ctx.assets.get<asset_type::music>(MENU_MUSIC);
  m_title_font = ctx.assets.get<asset_type::font>(TITLE_FONT, font_size);
  PlayMusicStream(m_music);
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

struct start_screen_json_data {
  std::string title;
  std::string subtitle;
  std::string text_key;
  int font_size = 0;
};
void start_screen::on_render() {
  ClearBackground(colors::darkblue);
  m_space_background.draw();

  static const auto json_cfg =
      ctx.assets.get_config<start_screen_json_data>("assets/start_screen.json");
  [&]() {
    auto [w, h] = ctx.app_info.game_res;

    const auto text_size =
        MeasureTextEx(m_title_font, json_cfg.title.c_str(), font_size, 0.f);
    const Vector2 text_pos{
        w / 2 - text_size.x / 2,
        50,
    };
    // draw a brown rectangle behind the text
    const auto title_rect = Rectangle{
        text_pos.x - 10,
        text_pos.y,
        text_size.x + 10,
        text_size.y + 10,
    };
    DrawRectangleRec(title_rect, base_color);
    DrawTextEx(m_title_font, json_cfg.title.c_str(), text_pos, font_size,
               text_space, colors::yellow);
  }();  // draw title
  const static auto start_text{"Press <space> to play"};
  auto blendingAlpha = [speed = 2.f, minAlpha = 0.2f,
                        maxAlpha = 1.f](float time) -> std::uint8_t {
    const float t = (std::sin(time * speed) + 1.0f) * 0.5f;
    return static_cast<std::uint8_t>(
        (minAlpha + t * (maxAlpha - minAlpha)) *
                                     255);
  };
  auto color = rooster::colors::yellow;
  color.a = blendingAlpha(float(GetTime()));
  DrawTextEx(m_title_font, start_text, {20, 100}, font_size, text_space, color);
}
}  // namespace scenes
