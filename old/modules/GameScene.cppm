export module GameScene;
import Game;
import sfml;
import stdbridge;
import components;
import LevelManager;
import EnemyFactory;
import tilemap;
import ShooterSelector;


export class GameScene : public Scene
{
  // Tilemap *m_curent_map;
  std::unique_ptr<LevelManager> m_levels;
  // std::unique_ptr<ShooterSelector> m_shooters;
  // std::string_view current_map = "1.tmx";
  constexpr static std::string_view levels_file = "levels.json";
  constexpr static std::string_view enemies_file = "enemies.json";
  std::function<void(float, const std::vector<sf::Vector2f> &)> m_enemy_system;
  std::function<void(float)> m_bullet_system;
  // std::vector<sf::Vector2f> m_spawn_points;


public:
  virtual void on_start() override;
  virtual void on_update(float delta) override;
  virtual void on_event(const sf::Event &e) override;
  void enemy_system(float delta);
  void bullet_system(float delta);
  virtual void on_render() override;

  std::unique_ptr<Scene> on_exit() override;
};
