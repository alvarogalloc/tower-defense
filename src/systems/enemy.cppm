export module systems.enemy;
import ginseng;
import components.enemy;
import components.movement;
import std;
import raylib;
import system_manager;

export namespace systems::enemy {
struct spawner_cfg { //NOLINT
  components::enemy enemy;
  components::bounding_box box;
  Vector2 spawn_point;
  int spawn_radius;
};

using spawner =
    std::function<ginseng::database::ent_id(ginseng::database &)>;

spawner make_spawner(const spawner_cfg &cfg);

// New unified system function with phase support
void system(ginseng::database &db, float dt, systems::Phase phase);

// Legacy functions for backward compatibility
void update(ginseng::database &db, float dt);
void draw(ginseng::database &db);
}  // namespace systems::enemy
