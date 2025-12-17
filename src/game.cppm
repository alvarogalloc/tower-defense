#include <entt/entity/fwd.hpp>
#include <type_traits>
export module game;
import entt;
import raylib;
import std;
import config;
import assets;

import debug;
#if 0
export {
  class game;
  class scene;

  class scene {
   public:
    bool m_should_exit = false;

    [[nodiscard]] bool should_exit() const { return m_should_exit; }
    [[nodiscard]] virtual bool should_exit_game() const { return false; }

    virtual void on_start() = 0;
    virtual void on_update() = 0;
    virtual void on_render() = 0;
    virtual std::unique_ptr<scene> on_exit() = 0;
    virtual ~scene() = default;
  };

  class game {
   public:
    game();
    static auto get() -> game &;
    void exit();
    void set_scene(std::unique_ptr<scene> scene);
    [[nodiscard]] int run();
    auto& push_debug_message(const debug::message &msg) {
      return m_debug_messages.emplace_back(msg);
    }

    auto get_debug_messages() -> std::vector<debug::message> & {
      return m_debug_messages;
    }
    [[nodiscard]] auto get_config() const -> config::config {
      return m_cfg;
    }
    auto get_world() -> entt::registry & { return m_world; }

   private:

    void draw_debug_messages() const;
    RenderTexture m_target{};
    config::config m_cfg;
    std::unique_ptr<scene> m_scene;
    entt::registry m_world;
    std::vector<debug::message> m_debug_messages;
  };
}

#endif

export {
  class game {
   public:
    game();
    void run();

   private:
    entt::registry m_ecs;
    config::config m_cfg;
    assets m_assets;
  };
}

static_assert(std::is_default_constructible<game>,
              "game should be default constructible");
