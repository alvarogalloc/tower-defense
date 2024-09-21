export module game;
import ginseng;
import raylib;
import std;

export
{
  class game;
  class scene;

  struct game_spec {
    std::string_view window_name;
    std::string_view asset_path;
    Vector2 win_size;
    int target_fps;
  };

  class scene {
protected:
    ginseng::database* m_world;
    bool m_should_exit = false;

public:
    bool should_exit() const
    {
      return m_should_exit;
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
    game(const game_spec& spec);
    void exit();
    auto get_spec() const -> const game_spec&
    {
      return m_spec;
    }
    auto get_world() -> ginseng::database&
    {
      return m_world;
    }
    void set_scene(std::unique_ptr<scene> scene);
    [[nodiscard]] int run();

private:
    game_spec m_spec;
    std::unique_ptr<scene> m_scene;
    ginseng::database m_world;
  };
}
