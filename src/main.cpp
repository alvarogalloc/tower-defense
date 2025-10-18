import game;
import std;
import scenes.start_screen;
import config;
import raylib;

int main() {
  const config::app_info game_info{
      .window_name = "Hello, World!",
      .asset_path = SRC_DIR "/assets",
      // .size{float(GetScreenWidth()/2), float(GetScreenHeight()/2)},
      // .game_res{float(GetScreenWidth()/2), float(GetScreenHeight()/2)},
      .size{640, 360},
      .game_res{640, 360},
      .fps = 60,
  };

  game mygame{game_info};
  mygame.set_scene(std::make_unique<scenes::start_screen>());
  return mygame.run();
}
