export module systems.enemy;
import ginseng;
import components.enemy;
import components.movement;
import std;
import raylib;

export namespace systems::enemy {
  using spawner = std::function<ginseng::database::ent_id(ginseng::database&, float, float)>;
  spawner make_spawner(const components::bounding_box box, const components::enemy enemy);
  void update(ginseng::database& db, float dt);
  void draw(ginseng::database& db);
} // namespace systems::enemy
