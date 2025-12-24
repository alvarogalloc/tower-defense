export module state_manager;
import std;
import state;
import debug;

export {
  class state_manager {
   public:
    state_manager() = default;
    explicit state_manager(std::unique_ptr<state> initial)
        : current(std::move(initial)) {}
    void unsafe_change(std::unique_ptr<state> next) {
      debug::my_assert(bool(next), "cannot change state to an empty one");
      current = std::move(next);
    }
    void change() {
      auto next = current->on_exit();
      if (bool(next)) {
        current = std::move(next);
        current->on_start();
      } else {
        current = nullptr;
      }
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

    [[nodiscard]] auto* get_current() {
      debug::my_assert(bool(current), "cannot get an empty state!!");
      return current.get();
    }

   private:
    std::unique_ptr<state> current;
  };
}
