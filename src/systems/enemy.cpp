module systems.enemy;
import components.enemy;
import systems.player;
import components.bullet;
import components.movement;
import components.misc;
import debug;
import std;
import raylib;
using namespace rooster;
namespace systems::enemy {
spawner make_spawner(const spawner_cfg &cfg) {
  auto &[enemy, box] = cfg;
  return [enemy, box](ginseng::database &db, float x, float y) {
    // this call represent a new enemy with the 'enemy' preset
    auto id = db.create_entity();
    const auto new_box = components::bounding_box{x, y, box.width, box.height};
    db.add_component(id, new_box);
    db.add_component(id, enemy);
    db.add_component(
        id,
        LoadTexture(std::format("{}/{}", SRC_DIR, enemy.texture_path).c_str()));
    return id;
  };
}

void update(ginseng::database &db, float dt) {
  const auto player_id = [&db] {
    std::optional<ginseng::database::ent_id> player_id;
    db.visit([&player_id](ginseng::database::ent_id id,
                          components::misc::player) { player_id = id; });
    debug::my_assert(bool(player_id), "player not found");
    return *player_id;
  }();

  const Vector2 player_pos =
      db.get_component<components::movement>(player_id).position;

  const auto hit_detection = [dt, player_pos](
                                 components::enemy &enemy,
                                 components::bounding_box &enemy_box) {
    Vector2 vec_enemy_to_player =
        Vector2Subtract(player_pos, {enemy_box.x, enemy_box.y});
    float distance_to_player = Vector2Length(vec_enemy_to_player);
    vec_enemy_to_player =
        Vector2Scale(Vector2Normalize(vec_enemy_to_player), enemy.speed * dt);
    if (distance_to_player < 100.f) {
      return;
    }

    auto [x, y] = Vector2Add({enemy_box.x, enemy_box.y}, vec_enemy_to_player);
    enemy_box.x = x;
    enemy_box.y = y;
  };
  db.visit(hit_detection);
}
void draw(ginseng::database &db) {
  db.visit([](components::enemy &e, components::bounding_box &box,
              const Texture2D &text) {
    const float thickness = 2.f;
    const auto color = colors::red;

    DrawRectangleLinesEx(box, thickness, color);
    DrawTextureEx(text, {box.x, box.y}, 0.f, 1.f, colors::white);
    const int tooltip_offset_px = 20;
    const int font_size = 12;
    DrawText(std::format("{}/{}", e.health, e.max_health).c_str(), int(box.x),
             int(box.y) - tooltip_offset_px, font_size, colors::white);
  });
}

}  // namespace systems::enemy
