import Game;
import MenuScene;
import stdbridge;

int main()
{
  GameSpec spec{ .window_name{ "Magster" },
    .asset_path{ std::string{ SRC_DIR } + "/assets/" },
    .win_size{ 640, 704 } };
  Game game(spec);
  game.set_scene(std::make_unique<MenuScene>());
  return game.run();
}
