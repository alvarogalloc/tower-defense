export module scenes.shooting;
import game;
import std;
import ginseng;
import raylib;
import systems.enemy;
import prefabs.stars_particles;
import scenes.gameover;
import systems.waves;
import system_manager;
import resource_manager;
import event_system;

export namespace scenes
{

class shooting : public scene
{
    // New architecture components
    systems::SystemManager m_system_mgr{};
    systems::ResourceManager m_resources{};
    systems::EventBus m_event_bus{};
    
    // Legacy system storage for waves (will be migrated later)
    std::vector<std::function<void(ginseng::database &, float)>> m_legacy_systems{};
    std::vector<systems::enemy::spawner> m_enemy_spawners{};
    std::vector<systems::waves::level_cfg> m_levels;

    // Scene-specific state
    prefabs::stars_particles m_particles;
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
