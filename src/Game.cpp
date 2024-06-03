module Game;
import fmt;
import utils;
import imgui;

void Scene::init(my_assets &manager,
  ginseng::database &world,
  sf::RenderWindow &win)
{
  m_manager = &manager;
  m_world = &world;
  m_win = &win;
}

void Game::on_update(float delta) { m_scene->on_update(delta); }
void Game::on_event(const sf::Event &ev)
{
  if (ev.type == sf::Event::Closed) { m_win.close(); }
  m_scene->on_event(ev);
}
void Game::on_render() { m_scene->on_render(); }
void Game::on_exit() { ImGui::SFML::Shutdown(); }

Game::Game(GameSpec spec)
  : m_manager(spec.asset_path),
    m_win(sf::VideoMode{ spec.win_size.x, spec.win_size.y }, spec.window_name)
{

  if (!ImGui::SFML::Init(m_win, false))
  {
    throw std::runtime_error{ "failed to initialize imgui" };
  }
  constexpr static auto font_path =
    "/System/Library/Fonts/Supplemental/Courier New Bold.ttf";
  auto &io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF(font_path, 32.0f);
  io.Fonts->AddFontFromFileTTF(font_path, 16.0f);
  my_assert(ImGui::SFML::UpdateFontTexture(), "could not build font atlas");
  m_win.setFramerateLimit(60);
  m_win.setKeyRepeatEnabled(false);
}

void Game::set_scene(std::unique_ptr<Scene> scene)
{
  m_scene.reset();
  m_scene = std::move(scene);
  m_scene->init(m_manager, m_world, m_win);
  m_scene->on_start();
}
int Game::run()
{
  try
  {
    my_assert(bool(m_scene), "no scene has been set");
    m_clock.restart();
    sf::Event ev;
    while (m_win.isOpen())
    {
      while (m_win.pollEvent(ev))
      {
        ImGui::SFML::ProcessEvent(ev);
        on_event(ev);
      }
      ImGui::SFML::Update(m_win, m_clock.getElapsedTime());
      on_update(m_clock.restart().asSeconds());
      if (m_scene->should_exit())
      {
        auto next = m_scene->on_exit();
        if (next.get() == nullptr) { break; }
        set_scene(std::move(next));
      }
      on_render();
      ImGui::EndFrame();
      ImGui::SFML::Render(m_win);
      m_win.display();
    }
    on_exit();
  } catch (const std::exception &e)
  {
    on_exit();
    fmt::println("exception caught when running the game: \n\t{}", e.what());
    return 1;
  }
  return 0;
}
