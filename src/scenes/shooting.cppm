export module scenes:shooting;
import std;
import entt;
import raylib;
import systems;
import prefabs;
import :gameover;
import wey;

export namespace scenes
{

class shooting : public state
{
    std::vector<std::function<void(entt::registry &, float)>> m_systems{};
    std::vector<std::variant<Texture2D, Sound>> m_to_clean{};
    std::vector<systems::enemy::spawner> m_enemy_spawners{};

    std::vector<systems::waves::level_cfg> m_levels;

    systems::enemy::spawner m_enemy_spawner{};
    prefabs::stars_particles m_particles;
    Camera2D m_camera;
    entt::entity m_player_entity;
    Sound m_gun_shoot_sfx;
    bool m_should_exit;

    game_stats m_stats;

  public:
    shooting(context_view);

    void on_start() override;
    void on_update() override;

    std::unique_ptr<state> on_exit() override;
    [[nodiscard]] bool should_exit() const override {return m_should_exit;}
    void on_render() override;
};
} // namespace scenes
