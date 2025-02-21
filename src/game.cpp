module game;
import debug;

game::game(config::app_info spec)
    : m_spec(std::move(spec))
{
  InitWindow(
    static_cast<int>(spec.size.x), static_cast<int>(spec.size.y), spec.window_name.data());
  SetTargetFPS(spec.fps);
  InitAudioDevice();
}
void game::exit()
{
  CloseAudioDevice();
  CloseWindow();
}
void game::set_scene(std::unique_ptr<scene> scene)
{
  this->m_scene.reset();
  this->m_scene = std::move(scene);
  this->m_scene->init(*this);
  this->m_scene->on_start();
}
int game::run()
{
  try {
    bool debug_mode = false;
    debug::my_assert(bool(m_scene), "Scene not set");
    while (!WindowShouldClose()) {
      if (m_scene->should_exit_game()) {
        break;
      }
      if (m_scene->should_exit()) {
        auto new_scene = m_scene->on_exit();
        if (!new_scene) {
          break;
        }
        this->set_scene(std::move(new_scene));
      }
      m_scene->on_update();
      // f1 to toggle debug
      debug_mode = IsKeyPressed(KEY_F1) ? !debug_mode : debug_mode;

      if (debug_mode) {
        debug::draw_debug(m_world);
      }
      BeginDrawing();
      {
        m_scene->on_render();
      }

      EndDrawing();
    }
    this->exit();
    return 0;
  } catch (const std::exception& e) {
    std::print("{}Error: {}\n{}", debug::error, e.what(), debug::reset);
    this->exit();
    return 1;
  }
}

void scene::init(game& game)
{
  m_world = &game.get_world();
}
