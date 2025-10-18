export module systems.waves;
import ginseng;
import systems.enemy;
import raylib;
import std;
constexpr static std::size_t invalid_enemy = 1e9;

export namespace systems::waves {

struct wave_cfg {
  std::vector<int> enemy_sequence;
  int count;
  float spawn_interval_seconds;
  float spawn_radius;
  std::size_t current_enemy_id = invalid_enemy;  // first invalid, so check
};
struct level_cfg {
  std::string title;
  std::vector<wave_cfg> waves;
  std::size_t current_wave = 0;
  float timer = 0.f;
};
void update_level(ginseng::database &, float dt, level_cfg &level,
                  const std::vector<systems::enemy::spawner_cfg> &) {
  // if all waves are finished, finish level, do nothing
  if (level.waves.size()-1 <= level.current_wave) {
    return;
  }

  // poll current wave
  auto& current_wave = level.waves[level.current_wave];
}
}  // namespace systems::waves
