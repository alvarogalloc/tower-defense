export module game_scene;
import game;
import std;
import raylib;

struct enemy {
  Vector2 position;
  int frame;
  int frame_counter;
  int frame_speed;
  int health;
  float speed;
};
struct bullet {
  int hit_count;
  int max_hit_count;
  int damage;
  Vector2 position;
  Vector2 velocity;
  float angle;
};

export class game_scene : public scene {
  private:
  Texture2D m_enemy_frames;
  Texture2D m_grass;
  Music m_music;
  std::array<enemy, 15> enemies;
  std::array<bullet, 10> bullets;

  public:
  void on_start() override;
  void on_update() override;

  std::unique_ptr<scene> on_exit() override;
  void on_render() override;
};
