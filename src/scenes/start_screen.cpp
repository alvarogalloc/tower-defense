module scenes.start_screen;
import scenes.shooting;
import config;
import gui;
import utils.assets_cache;
using namespace rooster;
inline constexpr int font_size = 48;
inline constexpr Color base_color{.r = colors::darkpurple.r,
                                  .g = colors::darkpurple.g,
                                  .b = colors::darkpurple.b,
                                  .a = 100};

constexpr auto TITLE_FONT = "/assets/monogram.ttf";
constexpr auto BLUE_GUY_TEXTURE = "/assets/blueguy.png";
constexpr auto MENU_MUSIC = "/assets/bgmusic.ogg";
inline constexpr float text_space = 0.2f;
namespace scenes {

start_screen::start_screen()
    : m_space_background(game::get().get_config().get_game_config().space_bg) {}
void start_screen::on_start() {
  constexpr float seconds_notification{15.f};
  game::get().push_debug_message({
      .lifetime_seconds = seconds_notification,
      .text = "Entered start screen scene",
  });

  m_title_font =
      LoadFontEx(game::get().get_config().get_path(TITLE_FONT).c_str(),
                 font_size, nullptr, 0);
  m_blue_guy = utils::get_asset<utils::asset_type::texture>(BLUE_GUY_TEXTURE);
  m_music = utils::get_asset<utils::asset_type::music>(MENU_MUSIC);
  PlayMusicStream(m_music);
}

void start_screen::on_update() {
  m_space_background.update();
  // if any key pressed
  if (GetKeyPressed() != 0) {
    m_should_exit = true;
  }
  UpdateMusicStream(m_music);
}

std::unique_ptr<scene> start_screen::on_exit() {
  UnloadTexture(m_blue_guy);
  // UnloadFont(m_title_font); font is shared with raygui so we don't unload
  // it
  return std::make_unique<scenes::shooting>();
}

void start_screen::on_render() {
  ClearBackground(colors::darkblue);
  m_space_background.draw();

  [&]() {
    const auto title =
        game::get().get_config().get_game_config().start_screen.title;
    auto [w, h] = game::get().get_config().get_app_info().game_res;

    const auto text_size =
        MeasureTextEx(m_title_font, title.c_str(), font_size, 0.f);
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
    DrawTextEx(m_title_font, title.c_str(), text_pos, font_size, text_space,
               colors::yellow);
  }();  // draw title
  // draw blending text saying 'Press any key to play'
  constexpr static auto start_text{"Press any key to play"};
  auto blendingAlpha = [](float time) -> std::uint8_t {
    // Pulse frequency (cycles per second)
    constexpr float speed = 2.0f;
    // Min and max alpha (0.0 to 1.0)
    constexpr float minAlpha = 0.2f;
    constexpr float maxAlpha = 1.0f;

    // Sine wave oscillation 0–1
    float t = (std::sin(time * speed) + 1.0f) * 0.5f;
    // Map to min-max and scale to 0–255
    return static_cast<std::uint8_t>((minAlpha + t * (maxAlpha - minAlpha)) *
                                     255);
  };
  auto color = rooster::colors::yellow;
  color.a = blendingAlpha(float(GetTime()));
  DrawTextEx(m_title_font, start_text, {20, 200}, font_size, text_space, color);
}
}  // namespace scenes
