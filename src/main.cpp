import game;
import std;
import scenes.menu;
import config;

int main()
{
    game mygame{config::game_info};
    mygame.set_scene(std::make_unique<scenes::menu>());
    return mygame.run();
}
