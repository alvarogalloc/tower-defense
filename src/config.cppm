export module config;
import std;
import raylib;
import debug;
import glaze;
export namespace config {

struct start_screen {
  std::string title;
  std::string subtitle;
  std::string text_key;
  int font_size;
};

struct gameover {
  Color bg_color;
  Color btn_color;
  Color fg_color;
  int font_size;
};

struct space_bg {
  std::string bg_texture;
  std::vector<std::string> objects_textures;
  float scroll_speed{};
};

// Splash screen with personalised msg
struct game_config {
  start_screen start_screen;
  gameover gameover;
  space_bg space_bg;
};

// actual window and application settings
struct app_info {
  std::string window_name;
  Vector2 game_res;
  float scale;
  int fps;
};
class config {
  std::string m_game_dir;
  app_info m_app_info;
  game_config m_game_config;
 public:
  config();
  auto get_game_dir() const -> std::string{
      return m_game_dir;
  }
  auto get_path(const std::string_view p) const -> std::string;
  auto get_app_info() const -> app_info { return m_app_info; }
  auto get_game_config() const -> game_config { return m_game_config; }
};

}  // namespace config
