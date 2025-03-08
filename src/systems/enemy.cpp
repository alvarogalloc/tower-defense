module systems.enemy;
import components.enemy;
import components.bullet;
import components.movement;
import std;
namespace systems::enemy {
  std::function<ginseng::database::ent_id(ginseng::database&, float, float)>
  make_spawner(components::bounding_box box, components::enemy enemy)
  {
    return [box, enemy](ginseng::database& db, float x, float y) {
      auto id = db.create_entity();
      const auto new_box = components::bounding_box {x, y, box.width, box.height};
      db.add_component(id, new_box);
      db.add_component(id, enemy);
      return id;
    };
  }

  void update(ginseng::database& db, float)
  {
    db.visit([&db](components::enemy& e, components::bounding_box& box) {
      std::println("Enemy update: {}", e.health);
      db.visit([&](ginseng::database::ent_id id, components::bullet& b) {});
    });
  }
  void draw(ginseng::database&) {}

} // namespace systems::enemy
