export module state;
import entt;
import config;

export {
  struct context {
    entt::registry& ecs;
    config::config& cfg;
  };

  struct state {
    context ctx;
    state(context& c) : ctx(c) {}
    virtual ~state() = default;

    virtual void on_enter() {}
    virtual void on_exit() {}

    virtual void update(const float dt) = 0;
    virtual void draw() = 0;
  };
}
