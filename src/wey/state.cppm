export module wey:state;
import :assets;
import :config;
import entt;
import std;

export {
  struct context_view {
    entt::registry& ecs;
    config::app_info& app_info;
    assets& assets;
  };

  struct state {
    context_view ctx;

    state(context_view view) : ctx(view) {}
    state(entt::registry& ecs, config::app_info& app_info, assets& assets)
        : ctx(ecs, app_info, assets) {}
    virtual ~state() = default;

    virtual void on_start() {}
    virtual std::unique_ptr<state> on_exit() { return nullptr; }

    virtual void on_update() = 0;
    [[nodiscard]] virtual bool should_exit() const { return false; };
    virtual void on_render() = 0;
  };
}
