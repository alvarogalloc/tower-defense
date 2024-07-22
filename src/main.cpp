import raylib;
import raygui;
import game;
import stdbridge;
import menu_scene;

int main()
{
  game_spec spec{
    .window_name = "Hello, World!",
    .asset_path = SRC_DIR "/assets",
    .win_size = { 640, 704 },
    .target_fps = 60,
  };
  game mygame(spec);
  mygame.set_scene(std::make_unique<menu_scene>());
  return mygame.run();
}
