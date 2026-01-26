export module systems:enemy;
import components;
import std;
import raylib;
import entt;
import wey;

export namespace systems::enemy {

// gettting this from json, maybe rename to [name]_json
struct spawner_cfg {  // NOLINT
  components::enemy enemy;
  components::bounding_box box;
  Vector2 spawn_point;
  int spawn_radius;
};

using spawner = std::function<entt::entity(entt::registry &)>;

spawner make_spawner(const spawner_cfg &cfg, context_view ctx);
void update(entt::registry &db, float dt);
void draw(entt::registry &db);
}  // namespace systems::enemy
