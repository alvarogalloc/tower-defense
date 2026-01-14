import wey;
import ut;

struct mock_state : state {
  using state::state;
  void on_start() override {}
  std::unique_ptr<state> on_exit() override { return nullptr; }

  void on_update() override {}
  [[nodiscard]] bool should_exit() const override { return false; }
  void on_render() override {}
};
int main() {
  using namespace boost::ut;

  state_manager sm;
  using namespace std::filesystem;
  game_context gc{"TEST_GAME_DIR"};
  "state_manager"_test = [&] {
    expect(throws([&sm] { return sm.get_current(); }))
        << "this should throw because no state is set";
    sm.unsafe_change(std::make_unique<mock_state>(context_view{
        .ecs = gc.ecs,
        .app_info = gc.app_info,
        .assets = gc.assets,
    }));
    expect(_b(sm.get_current() != nullptr)) << "Scene should be set by now";
    sm.change();
    expect(throws([&] { return sm.get_current(); }))
        << "Scene should come to an end";
  };
}
