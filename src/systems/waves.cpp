module systems;
namespace systems::waves {
auto make_level_update(std::vector<level_cfg> &levels,
                       const std::vector<systems::enemy::spawner> &spawners)
    -> std::function<void(entt::registry &db, float dt)> {
  return [&levels, &spawners, i = std::size_t(0)](entt::registry &db,
                                                  float dt) {
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
        auto enemy_spawner = spawners.at(spawner_id);
        enemy_spawner(db);
        wave.enemy_index++;
      } else {
        wave.enemy_index = 0;
      }
    }
  };
}

void update_level(entt::registry &, float, level_cfg &level,
                  const std::vector<systems::enemy::spawner_cfg> &) {
  // if all waves are finished, finish level, do nothing
  if (level.waves.size() - 1 <= level.current_wave) {
    return;
  }

  // poll current wave
  // auto& current_wave = level.waves[level.current_wave];
}

}  // namespace systems::waves
