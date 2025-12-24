import game;
import scenes.start_screen;
import std;
import state;

int main() {
  game mygame{"MAGSTER_DIR"};
  auto& [ecs, app_info, assets] = mygame.get_context();
  mygame.run(std::make_unique<scenes::start_screen>(context_view{
      .ecs = ecs,
      .app_info = app_info,
      .assets = assets,
  }));
}
