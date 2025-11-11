import game;
import scenes.start_screen;
import config;
import glaze;
import debug;
import std;

int main() {
  config::app_info game_info{};
  const auto game_info_path{
      std::format("{}/assets/game_info.json", config::src_dir())};
  std::string b;
  const auto ec = glz::read_file_json(game_info, game_info_path, b);
  if (ec) {
    debug::my_assert(false, std::format("failed to load game info, error {}",
                                        glz::format_error(ec)));
  } else {
    std::cout << "game info loaded" << b << "\n";
  }

  game mygame{game_info};
  mygame.set_scene(std::make_unique<scenes::start_screen>());
  return mygame.run();
}
