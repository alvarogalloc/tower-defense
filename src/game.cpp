module game;
import fmt;
import debug;


game::game(const game_spec &spec) : m_spec(spec)
{
  InitWindow(static_cast<int>(spec.win_size.x),
    static_cast<int>(spec.win_size.y),
    spec.window_name.data());
  SetTargetFPS(spec.target_fps);
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
  try
  {

    my_assert(bool(m_scene), "Scene not set");
    while (!WindowShouldClose())
    {
      if (m_scene->should_exit())
      {
        auto new_scene = m_scene->on_exit();
        if (!new_scene) { break; }
        this->set_scene(std::move(new_scene));
      }
      m_scene->on_update();
      BeginDrawing();
      {
        m_scene->on_render();
      }

      EndDrawing();
    }
    this->exit();
    return 0;
  } catch (const std::exception &e)
  {
    fmt::print(error, "Error: {}\n", e.what());
    this->exit();
    return 1;
  }
}

void scene::init(game &game) { m_world = &game.get_world(); }