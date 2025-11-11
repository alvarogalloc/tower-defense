import game;
import std;
import scenes.start_screen;
import config;
import glaze;
import debug;

int main() {
  config::app_info game_info{};
  const auto game_info_path{SRC_DIR "/assets/game_info.json"};
  std::string b{};
  const auto ec = glz::read_file_json(game_info, game_info_path, b);
  if (ec) {
    debug::my_assert(false, std::format("failed to load game info, error {}",
                                        glz::format_error(ec)));
  } else {
    std::println("game info loaded  {}", b);
  }

  game mygame{game_info};
  mygame.set_scene(std::make_unique<scenes::start_screen>());
  return mygame.run();
}
