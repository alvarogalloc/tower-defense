module systems.enemy;
import components.enemy;
import components.bullet;
import components.movement;
import components.tags;
import std;
import raylib;
namespace systems::enemy {
  std::function<ginseng::database::ent_id(ginseng::database&, float, float)>
  make_spawner(const components::bounding_box box, const components::enemy enemy)
  {
    return [box, enemy](ginseng::database& db, float x, float y) {
      // this call represent a new enemy with the 'enemy' preset
      auto id = db.create_entity();
      const auto new_box = components::bounding_box {x, y, box.width, box.height};
      db.add_component(id, new_box);
      db.add_component(id, enemy);
      return id;
    };
  }

  void update(ginseng::database& db, float dt)
  {
    Vector2 player_pos = [&db] {
      Vector2 player_pos = {0.f, 0.f};
      db.visit([&db, &player_pos](ginseng::database::ent_id id, components::tags::player) {
        player_pos =  db.get_component<components::movement>(id).position;
      });
      return player_pos;
    }();
    // std::println("player pos: (x: {}, y: {})", player_pos.x, player_pos.y);
    db.visit([dt, player_pos](components::enemy& e, components::bounding_box& b) {
      std::println("{}", e.to_string());
      Vector2 direction = Vector2Subtract(player_pos, {b.x, b.y});
      float distance = Vector2Length(direction);
      if (distance > 0) {
        direction = Vector2Scale(Vector2Normalize(direction), e.speed * dt);
      }
      auto[x,y] = Vector2Add({b.x, b.y}, direction);
      b.x = x;
      b.y = y;

    });
  }
  void draw(ginseng::database& db) {
    db.visit([](components::enemy& e, components::bounding_box&box ) {
      const float thickness = 2.f;
      const auto color = colors::red;

      DrawRectangleLinesEx(box, thickness, color);
      DrawTextureEx(e.texture , {box.x, box.y}, 0.f, 1.f, colors::white);
    });
  }


} // namespace systems::enemy
