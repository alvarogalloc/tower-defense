module systems.enemy;
import components.enemy;
import systems.player;
import components.bullet;
import utils.assets_cache;
import components.movement;
import components.misc;
import debug;
import glaze;
import std;
import raylib;
import config;
import entt;

using namespace rooster;
namespace systems::enemy {
spawner make_spawner(const spawner_cfg &cfg) {
  return [cfg](entt::registry &db) {
    auto &[enemy, box, pos, rad] = cfg;
    std::string pretty;
    auto _ = glz::write<glz::opts{.prettify = true}>(cfg, pretty);
    std::println(" loading enemy with info {}", pretty);

    // this call represent a new enemy with the 'enemy' preset
    auto id = db.create();
    const auto new_box = components::bounding_box{
        pos.x + float(GetRandomValue(-rad, rad)),
        pos.y + float(GetRandomValue(-rad, rad)),
        box.width,
        box.height,
    };
    std::println("making a new enemy at {}, {}", new_box.x, new_box.y);
    db.emplace<Rectangle>(id, new_box);
    db.emplace<components::enemy>(id, enemy);

    using namespace utils;
    db.emplace<Texture>(id,
                        // this neeeds cache
                        get_asset<asset_type::texture>(enemy.texture_path));
    return id;
  };
}

void update(entt::registry &db, float dt) {
  const auto player_id = [&db] {
    auto view = db.view<const components::misc::player>();
    debug::my_assert(bool(!view->empty()), "player not found");
    return *view->begin();
  }();

  const Vector2 player_pos = db.get<components::movement>(player_id).position;

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

  auto view = db.view<components::enemy, components::bounding_box>();
  view.each(hit_detection);
}
void draw(entt::registry &db) {
  auto view =
      db.view<components::enemy, components::bounding_box, const Texture2D>();
  view.each([](components::enemy &e, components::bounding_box &box,
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
