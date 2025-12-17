export module state_manager;
import std;
import state;

export  {

class state_manager {
 public:
  void change(std::unique_ptr<state> next) {
    if (current) current->on_exit();
    current = std::move(next);
    current->on_enter();
  }

  void update(double dt) {
    if (current) current->update(dt);
  }
  void draw() {
    if (current) current->draw();
  }

 private:
  std::unique_ptr<state> current{nullptr};
};
}
