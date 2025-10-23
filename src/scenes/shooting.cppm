export module scenes.shooting;
import game;
import std;
import ginseng;
import raylib;
import systems.enemy;
import prefabs.stars_particles;
import scenes.gameover;
import systems.waves;

export namespace scenes
{

class shooting : public scene
{
    std::vector<std::function<void(ginseng::database &, float)>> m_systems{};
    std::vector<std::variant<Texture2D, Sound>> m_to_clean{};
    std::vector<systems::enemy::spawner> m_enemy_spawners{};

    std::vector<systems::waves::level_cfg> m_levels;

    systems::enemy::spawner m_enemy_spawner{};
    prefabs::stars_particles m_particles;
    Camera2D m_camera;
    ginseng::database::ent_id m_player_entity;
    Sound m_gun_shoot_sfx;

    game_stats m_stats;

  public:
    shooting();

    void on_start() override;
    void on_update() override;

    std::unique_ptr<scene> on_exit() override;
    void on_render() override;
};
} // namespace scenes
