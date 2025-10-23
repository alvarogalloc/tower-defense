export module systems.waves;
import ginseng;
import systems.enemy;
import raylib;
import std;
import debug;
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

auto make_level_update( std::vector<level_cfg> &levels,
                       const std::vector<systems::enemy::spawner> &spawners) -> std::function<void(ginseng::database &db,float dt)> {
  return [&levels,&spawners, i = std::size_t(0)](ginseng::database &db,float dt)  {
    debug::my_assert(i < levels.size(), "out of bounds level");
    auto &curr = levels.at(i);
    auto &wave = curr.waves.at(curr.current_wave);
    if (curr.timer < wave.spawn_interval_seconds) {
      curr.timer += dt;
    } else {
      curr.timer = 0.f;
      // we should put a new guy up there
      if (!wave.count) {
        std::puts("Go to the next wave one!!\n");
      }
      wave.count--;
      if (wave.enemy_index < wave.enemy_sequence.size()) {
        auto spawner_id = wave.enemy_sequence[wave.enemy_index];
        debug::my_assert(spawner_id < spawners.size(), "index out of bounds");
        std::println("el id es {}", spawner_id);
        auto enemy_spawner =spawners.at( spawner_id );
        enemy_spawner(db);
        wave.enemy_index++;
      } else {

        wave.enemy_index = 0;
      }
    }
  };
}

void update_level(ginseng::database &, float, level_cfg &level,
                  const std::vector<systems::enemy::spawner_cfg> &) {
  // if all waves are finished, finish level, do nothing
  if (level.waves.size() - 1 <= level.current_wave) {
    return;
  }

  // poll current wave
  // auto& current_wave = level.waves[level.current_wave];
}
}  // namespace systems::waves
