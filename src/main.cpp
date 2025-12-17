import game;
import scenes.start_screen;
import std;

int main() {
  game mygame{};
  mygame.set_scene(std::make_unique<scenes::start_screen>());
  return mygame.run();
}
