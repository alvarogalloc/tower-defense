export module state_manager;
import std;
import state;

export {
  class state_manager {
   public:
    state_manager(std::unique_ptr<state> initial)
        : current(std::move(initial)) {}
    void change() {
      auto next = current->on_exit();
      if (bool(next)) {
        current = std::move(next);
      }
      current->on_start();
    }

    void update() {
      if (current) {
        if (current->should_exit()) change();
        current->on_update();
      }
    }
    void render() {
      if (current) current->on_render();
    }

   private:
    std::unique_ptr<state> current;
  };
}
