module;
#include <SFML/OpenGL.hpp>
#include <imgui.h>
#include <imgui_impl_opengl2.h>

module Game;
import fmt;
import utils;

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
void Game::on_exit() {}

Game::Game(GameSpec spec)
  : m_manager(spec.asset_path),
    m_win(sf::VideoMode{ spec.win_size.x, spec.win_size.y },
      spec.window_name,
      sf::Style::Close,
      sf::ContextSettings(24, 8, 4, 2, 1))
{
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
    m_win.setVerticalSyncEnabled(true);
    m_win.setActive(true);
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.DisplaySize = ImVec2(640, 704);
    io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;// Enable Keyboard Controls
    io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad;// Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGui_ImplOpenGL2_Init();


    my_assert(bool(m_scene), "no scene has been set");
    m_clock.restart();
    sf::Event ev;
    while (m_win.isOpen())
    {

      while (m_win.pollEvent(ev)) { on_event(ev); }

      on_update(m_clock.restart().asSeconds());
      if (m_scene->should_exit())
      {
        auto next = m_scene->on_exit();
        if (!next)
        {
          fmt::println("no scene returned from on_exit, exiting");
          break;
        }
        set_scene(std::move(next));
      }

      ImGui_ImplOpenGL2_NewFrame();
      // ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      on_render();

      ImGui::Begin("Hello, world!");
      ImGui::Button("Look at this pretty button");
      ImGui::End();
      ImGui::Render();
      glViewport(0, 0, m_win.getSize().x, m_win.getSize().y);

      float clear_color[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
      glClearColor(
        clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
      glClear(GL_COLOR_BUFFER_BIT);

      ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
      m_win.display();
    }
    ImGui_ImplOpenGL2_Shutdown();
    ImGui::DestroyContext();
    on_exit();
  } catch (const std::exception &e)
  {
    on_exit();
    fmt::println("exception caught when running the game: \n\t{}", e.what());
    return 1;
  }
  return 0;
}
