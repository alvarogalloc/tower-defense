module;
#ifndef SRC_DIR
#include <cstdlib>
#endif
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

struct game_config {
  start_screen start_screen;
  gameover gameover;
  space_bg space_bg;
};
constexpr auto src_dir = []() -> std::string {
  static const auto dir =
#ifdef SRC_DIR
      SRC_DIR;
#else
      getenv("MAGSTER_DIR");
#endif
  return dir;
};


inline auto get_path(const std::string_view p, std::source_location sl = std::source_location::current()) -> std::string {
  const auto s = std::format("{}/{}",src_dir(), p);
  debug::my_assert(std::filesystem::exists(std::filesystem::path{s}), std::format("path {} not found", s), sl);

  return s;
}

auto get_config() {
  static std::optional<game_config> config;
  if (config.has_value()) {
    return config.value();
  }

  const std::string config_file_path =
      get_path("/assets/game_config.json");
  std::println("Loading game config from {}", config_file_path);
  using namespace glz;

  std::string buf;
  auto ec = read_file_json(config, config_file_path, buf);

  if (ec) {
    debug::my_assert(false, std::format("Error parsing configuration file: {}",
                                        format_error(ec, buf)));
    config.reset();  // Reset to empty on error
  } else {
    std::println("Configuration loaded successfully.");
  }
  return config.value();
}

struct app_info {
  std::string_view window_name;
  Vector2 game_res;
  float scale;
  int fps;
};

}  // namespace config
