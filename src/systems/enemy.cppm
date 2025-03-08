export module systems.enemy;
import ginseng;
import components.enemy;
import components.movement;
import std;

export namespace systems::enemy {

  std::function<ginseng::database::ent_id(ginseng::database&, float, float)>
  make_spawner(components::bounding_box box, components::enemy enemy);

  void update(ginseng::database& db, float dt);

  void draw(ginseng::database& db);
} // namespace systems::enemy
