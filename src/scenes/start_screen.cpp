module scenes;
import :shooting;
import wey;
import gui;
import raylib;
import glaze;
import std;
using namespace rooster;

namespace scenes {
start_screen::start_screen(context_view view)
    : state(view), m_space_background("assets/space_config.json", view) {}

void start_screen::on_start() {
  json_cfg =
      ctx.assets.get_config<start_screen_json_data>("assets/start_screen.json");
  m_blue_guy = ctx.assets.get<asset_type::texture>("assets/blueguy.png");
  m_music = ctx.assets.get<asset_type::music>("assets/bgmusic.ogg");
  m_title_font = ctx.assets.get<asset_type::font>("assets/monogram.ttf",
                                                  json_cfg.title_font_size);
  PlayMusicStream(m_music);
  GuiSetFont(m_title_font);
  // build gui

  gui_root.add_child(gui::v2::label{json_cfg.title,
                                    rooster::colors::yellow,
                                    static_cast<float>(json_cfg.title_font_size),
                                    {10, 10}});

  gui_root.add_child(
      gui::v2::label{json_cfg.subtitle, json_cfg.text_color, 18, {0, 0}});
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
  json_cfg.text_color.a = blendingAlpha(float(GetTime()));

  const Vector2 text_position{20, 100};
  const float text_space = 0.2f;

  DrawTextEx(m_title_font, start_text, text_position,
             json_cfg.subtitle_font_size, text_space, json_cfg.text_color);
}
void start_screen::on_render() {
  ClearBackground(json_cfg.bg_color);
  m_space_background.draw();
  draw_blending_text();
  gui_root.draw();
}
}  // namespace scenes
