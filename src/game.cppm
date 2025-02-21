export module game;
import ginseng;
import raylib;
import std;
import config;

export
{
  class game;
  class scene;

  class scene {
protected:
    ginseng::database* m_world;
    bool m_should_exit = false;

public:
    bool should_exit() const
    {
      return m_should_exit;
    }
    virtual bool should_exit_game() const
    {
      return false;
    }

    void init(game& game);
    virtual void on_start() = 0;
    virtual void on_update() = 0;
    virtual void on_render() = 0;
    virtual std::unique_ptr<scene> on_exit() = 0;
    virtual ~scene() = default;
  };

  class game {
public:
    game(config::app_info spec);
    void exit();
    auto get_world() -> ginseng::database&
    {
      return m_world;
    }
    void set_scene(std::unique_ptr<scene> scene);
    [[nodiscard]] int run();

private:
    config::app_info m_spec;
    std::unique_ptr<scene> m_scene;
    ginseng::database m_world;
  };
}
