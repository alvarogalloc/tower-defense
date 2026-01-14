export module wey:config;
import std;
import raylib;
export namespace config {

struct app_info {
  std::string window_name;
  Vector2 game_res;
  float scale;
  int fps;
};
}  // namespace config
