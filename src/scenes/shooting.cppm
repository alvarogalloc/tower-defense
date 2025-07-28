export module scenes.shooting;
import game;
import std;
import ginseng;
import raylib;
import systems.enemy;
import prefabs.stars_particles;
import scenes.gameover;

export namespace scenes {

  class shooting : public scene {
    std::vector<std::function<void(ginseng::database&, float)>> m_systems{};
    std::vector<Texture2D> m_to_clean{};
    systems::enemy::spawner m_enemy_spawner{};
    prefabs::stars_particles m_bg;
    Camera2D m_camera;
    ginseng::database::ent_id m_player_entity;



    game_stats m_stats;

    
public:
    shooting();

    void on_start() override;
    void on_update() override;

    std::unique_ptr<scene> on_exit() override;
    void on_render() override;
  };
} // namespace scenes
