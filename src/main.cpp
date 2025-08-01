import game;
import std;
import scenes.start_screen;
import config;

int main()
{
    game mygame{config::game_info};
    mygame.set_scene(std::make_unique<scenes::start_screen>());
    return mygame.run();
}
