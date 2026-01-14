export module scenes:gameover;
import wey;
import raylib;
import std;

export namespace scenes {
struct game_stats {
  bool won;
  int score;
  int time_seconds;
};

class gameover : public state {
 private:
  game_stats stats;
  bool m_should_exit{false};

 public:
  gameover(context_view view, game_stats stats) : state(view), stats(stats) {}
  void on_start() override;
  void on_update() override;
  [[nodiscard]] std::unique_ptr<state> on_exit() override { return nullptr; }
  void on_render() override;
};
}  // namespace scenes
