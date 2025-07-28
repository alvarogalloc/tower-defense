export module scenes.gameover;
import game;
import raylib;
import std;

export namespace scenes
{
struct game_stats
{
    bool won;
    int score;
    int time_seconds;
};

class gameover : public scene
{
  private:
    game_stats stats;

  public:
    gameover(const game_stats);
    void on_start() override;
    void on_update() override;

    std::unique_ptr<scene> on_exit() override
    {
        return nullptr;
    }
    void on_render() override;
};
} // namespace scenes
