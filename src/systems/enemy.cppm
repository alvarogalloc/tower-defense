export module systems.enemy;
import components.enemy;
import components.movement;
import std;
import raylib;
import entt;

export namespace systems::enemy {
struct spawner_cfg { //NOLINT
  components::enemy enemy;
  components::bounding_box box;
  Vector2 spawn_point;
  int spawn_radius;
};

using spawner =
    std::function<entt::entity(entt::registry &)>;

spawner make_spawner(const spawner_cfg &cfg);
void update(entt::registry &db, float dt);
void draw(entt::registry &db);
}  // namespace systems::enemy
