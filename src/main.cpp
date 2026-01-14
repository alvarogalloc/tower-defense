import wey;
import scenes;
import std;

int main() {
  game mygame{"MAGSTER_DIR"};
  auto& [ecs, app_info, assets] = mygame.get_context();
  mygame.run(std::make_unique<scenes::start_screen>(context_view{
      .ecs = ecs,
      .app_info = app_info,
      .assets = assets,
  }));
}
