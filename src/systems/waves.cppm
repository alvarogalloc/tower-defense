export module systems:waves;
import entt;
import :enemy;
import raylib;
import std;
import wey;
export namespace systems::waves {

struct wave_cfg {
  std::vector<std::size_t> enemy_sequence;
  int count;
  float spawn_interval_seconds;
  std::size_t enemy_index = 0;  // first invalid, so check enemy_sequence[0]
};
struct level_cfg {
  std::string title;
  std::vector<wave_cfg> waves;
  std::size_t current_wave = 0;
  float timer = 0.f;
};

auto make_level_update(std::vector<level_cfg> &levels,
                       const std::vector<systems::enemy::spawner> &spawners)
    -> std::function<void(entt::registry &db, float dt)>;

void update_level(entt::registry &, float, level_cfg &level,
                  const std::vector<systems::enemy::spawner_cfg> &);
}  // namespace systems::waves
