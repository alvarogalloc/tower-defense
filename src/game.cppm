export module game;
import entt;
import raylib;
import std;
import config;
import assets;
import state_manager;
import state;
import debug;

export {
  struct game_context {
    game_context(std::string_view game_dir_env_var) : assets(game_dir_env_var) {}
    entt::registry ecs;
    config::app_info app_info;
    assets assets;
  };

  class game {
   public:
    game(std::string_view game_dir_env_var) : m_context(game_dir_env_var), m_state(nullptr) { init(); }
    void init();

    void run(std::unique_ptr<state> first_state);

    game_context& get_context() { return m_context; }
    entt::registry& get_ecs() { return m_context.ecs; }
    config::app_info& get_app_info() { return m_context.app_info; }
    assets& get_assets() { return m_context.assets; }

   private:
    game_context m_context;
    state_manager m_state;
  };
}
