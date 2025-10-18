export module systems.enemy;
import ginseng;
import components.enemy;
import components.movement;
import std;
import raylib;

export namespace systems::enemy {
struct spawner_cfg { //NOLINT
  components::enemy enemy;
  components::bounding_box box;
};

using spawner =
    std::function<ginseng::database::ent_id(ginseng::database &, float, float)>;

spawner make_spawner(const spawner_cfg &cfg);
void update(ginseng::database &db, float dt);
void draw(ginseng::database &db);
}  // namespace systems::enemy
