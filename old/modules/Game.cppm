export module Game;
import stdbridge;
import sfml;
import ginseng;
import assets;
import components;


export
{
  struct GameSpec;
  class Game;
  class Scene;

  struct GameSpec
  {
    std::string window_name;
    std::string asset_path;
    sf::Vector2u win_size;
  };

  class Scene
  {
  protected:
    my_assets *m_manager;
    ginseng::database *m_world;
    sf::RenderWindow *m_win;
    bool m_should_exit = false;


  public:
    bool should_exit() const { return m_should_exit; }
    void
      init(my_assets &manager, ginseng::database &world, sf::RenderWindow &win);
    virtual void on_start() = 0;
    virtual void on_update(float delta) = 0;
    virtual void on_event(const sf::Event &) = 0;
    virtual void on_render() = 0;
    virtual std::unique_ptr<Scene> on_exit() = 0;
    virtual ~Scene() = default;
  };

  class Game
  {
    my_assets m_manager;
    ginseng::database m_world;
    sf::RenderWindow m_win;
    std::unique_ptr<Scene> m_scene;
    sf::Clock m_clock;

    void on_update(float delta);
    void on_event(const sf::Event &);
    void on_render();
    void on_exit();

  public:
    Game(GameSpec spec);
    void set_scene(std::unique_ptr<Scene> scene);
    [[nodiscard]] int run();
  };
}
