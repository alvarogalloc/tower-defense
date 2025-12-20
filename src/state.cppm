export module state;
import entt;
import config;
import std;
import assets;

export {
  struct context_view {
    entt::registry& ecs;
    config::config& config;
    assets& assets;
  };

  struct state {
    context_view ctx;
    state(auto& ecs, auto& config, auto& assets)
        : ctx(ecs, config, assets) {}
    virtual ~state() = default;

    virtual void on_start() {}
    virtual std::unique_ptr<state> on_exit() const { return nullptr; }

    virtual void on_update() = 0;
    virtual bool should_exit() const { return false; };
    virtual void on_render() const = 0;
  };
}
