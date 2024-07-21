import raylib;
import raygui;
import rapidjson;
import stdbridge;
import fmt;
import ginseng;
import assets;

constexpr auto warn = fmt::emphasis::bold | fmt::fg(fmt::color::yellow);
constexpr auto error = fmt::emphasis::bold | fmt::fg(fmt::color::red);
constexpr auto info = fmt::emphasis::bold | fmt::fg(fmt::color::cyan);
constexpr auto success = fmt::emphasis::bold | fmt::fg(fmt::color::green);

void my_assert(bool condition,
  std::string_view message,
  std::source_location loc = std::source_location::current())
{
  if (!condition)
  {
    auto msg = fmt::format(
      "assertion failed: {} (at {}:{})", message, loc.file_name(), loc.line());
    throw std::runtime_error(msg);
  }
}
struct GameSpec
{
  std::string_view window_name;
  std::string_view asset_path;
  Vector2 win_size;
  int target_fps;
};
class Game;
class Scene;

class Scene
{
protected:
  my_assets *m_manager;
  ginseng::database *m_world;
  bool m_should_exit = false;


public:
  bool should_exit() const { return m_should_exit; }
  void init(Game &game);
  virtual void on_start() = 0;
  virtual void on_update() = 0;
  virtual void on_render() = 0;
  virtual std::unique_ptr<Scene> on_exit() = 0;
  virtual ~Scene() = default;
};

class Game
{
public:
  Game(const GameSpec &spec) : m_spec(spec), m_manager(spec.asset_path)
  {
    InitWindow(spec.win_size.x, spec.win_size.y, spec.window_name.data());
    SetTargetFPS(spec.target_fps);
    InitAudioDevice();
  }
  void exit()
  {
    CloseWindow();
    CloseAudioDevice();
  }

  auto get_spec() const -> const GameSpec & { return m_spec; }
  auto get_manager() -> my_assets & { return m_manager; }
  auto get_world() -> ginseng::database & { return m_world; }

  void set_scene(std::unique_ptr<Scene> scene)
  {
    m_scene.reset();
    m_scene = std::move(scene);
    m_scene->init(*this);
    m_scene->on_start();
  }

  [[nodiscard]] int run()
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
          set_scene(std::move(new_scene));
        }
        m_scene->on_update();
        BeginDrawing();
        {
          m_scene->on_render();
        }

        EndDrawing();
      }
      exit();
      return 0;
    } catch (const std::exception &e)
    {
      fmt::print(error, "Error: {}\n", e.what());
      exit();
      return 1;
    }
  }

private:
  GameSpec m_spec;
  std::unique_ptr<Scene> m_scene;
  my_assets m_manager;
  ginseng::database m_world;
};


void Scene::init(Game &game)
{
  m_manager = &game.get_manager();
  m_world = &game.get_world();
}


class MenuScene : public Scene
{
  Music *m_music;
  Texture2D *m_background;
  Font* m_big_font;

public:
  void on_start() override
  {
    m_background = m_manager->get<Texture2D>("tiles.png");
    m_music = m_manager->get<Music>("bgmusic.ogg");
    PlayMusicStream(*m_music);
    m_big_font =m_manager->get<Font>("monogram.ttf");
    GuiSetFont(*m_big_font);
  }
  void on_update() override
  {
    UpdateMusicStream(*m_music);
    if (IsKeyPressed(KEY_ENTER)) { m_should_exit = true; }
  }

  std::unique_ptr<Scene> on_exit() override
  {
    StopMusicStream(*m_music);
    return nullptr;
  }

  void on_render() override
  {
    auto centerh = [](const Rectangle &rect) {
      auto width = GetScreenWidth();
      return Rectangle{
        float(width) / 2 - rect.width / 2, rect.y, rect.width, rect.height
      };
    };
    auto centerv = [](const Rectangle &rect) {
      auto height = GetScreenHeight();
      return Rectangle{
        rect.x, float(height) / 2 - rect.height / 2, rect.width, rect.height
      };
    };
    // DrawTexture(*m_background, 0, 0, colors::white);
    ClearBackground(colors::darkgreen);
    DrawTextEx(
      *m_big_font, "Magster", { 600 / 2, 0 }, 120, 0.2f, colors::yellow);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 96);
    if (GuiButton(centerv(centerh({ 10, 40, 240, 80 })), "START"))
    {
      m_should_exit = true;
    }
    GuiSetStyle(DEFAULT, TEXT_SIZE, 16);
    if (GuiButton(centerh({ 10, 400, 80, 40 }), "EXIT"))
    {
      m_should_exit = true;
    }
  }
};

int main()
{
  GameSpec spec{
    .window_name = "Hello, World!",
    .asset_path = SRC_DIR "/assets",
    .win_size = { 800, 600 },
    .target_fps = 60,
  };
  Game game(spec);
  game.set_scene(std::make_unique<MenuScene>());
  return game.run();
}
