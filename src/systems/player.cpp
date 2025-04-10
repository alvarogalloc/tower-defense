module systems.player;
import systems.bullet;
import components.bullet;

namespace {
  using namespace systems::player;
  auto get_player_action() -> action
  {
    // can hold for continuous action
    if (IsKeyPressed(KEY_SPACE)) {
      return action::shoot;
    }
    if (IsKeyPressed(KEY_R)) {
      return action::reload;
    }
    return action::none;
  }

} // namespace

namespace systems::player {
  using namespace components;
  void update(ginseng::database& db, float dt)
  {
    db.visit([&](movement& player) {
      const auto starting_pos = player.position;
      Vector2 thrust = {0.0f, 0.0f};
      constexpr static float rotation_factor = 3.14f / 100.f;
      constexpr static float acceleration_factor = 1.6f;
      const auto sin_value = std::sin(player.rotation - 3.14f / 2.f);
      const auto cos_value = std::cos(player.rotation - 3.14f / 2.f);
      if (IsKeyDown(KEY_W)) {
        thrust = Vector2Scale({cos_value, sin_value}, acceleration_factor);
      }
      if (IsKeyDown(KEY_S)) {
        thrust = Vector2Scale({cos_value, sin_value}, -acceleration_factor);
      } // Down
      if (IsKeyDown(KEY_A)) {
        player.rotation -= rotation_factor;
      } // Left
      if (IsKeyDown(KEY_D)) {
        player.rotation += rotation_factor;
      } // Right
      if (IsKeyPressed(KEY_R)) {
        player.position = starting_pos;
      }
      if (thrust.x != 0.0f || thrust.y != 0.0f) {
        thrust = Vector2Scale(
          Vector2Normalize(thrust), player.acceleration_rate); // Apply acceleration
        player.velocity = Vector2Add(player.velocity, thrust);
        if (Vector2Length(player.velocity) > player.max_speed) {
          player.velocity
            = Vector2Scale(Vector2Normalize(player.velocity), player.max_speed); // Clamp velocity
        }
      } else {
        // Decelerate when no thrust is applied
        Vector2 deceleration
          = Vector2Scale(Vector2Normalize(player.velocity), -player.deceleration_rate);
        player.velocity = Vector2Add(player.velocity, deceleration);
        // Stop if velocity is very low
        if (Vector2Length(player.velocity) < 10.0f) {
          player.velocity = {0.0f, 0.0f};
        }
      }
      // Update position
      player.position = Vector2Add(player.position, Vector2Scale(player.velocity, dt));
    });

    db.visit([&](ginseng::database::ent_id entity, action& action, gun& gun) {
      // get action
      action = get_player_action();
      // then poll current action
      switch (action) {
        case action::shoot: {
          if (gun.ammo <= 0) return;
          gun.last_shot = float(GetTime());
          gun.ammo -= 1;
          const auto& player = db.get_component<movement>(entity);
          systems::bullet::shoot_bullet(
            db,
            components::bullet {
              player.position,
              Vector2Scale(Vector2Normalize(player.velocity), 100.f),
              player.rotation,
              10,
              4.5
            });

          break;
        }
        case action::reload: gun.ammo = gun.max_ammo; break;
        case action::none: break;
      }
    });
  }

  void draw(ginseng::database& db)
  {
    db.visit([&](ginseng::database::ent_id id, movement& player, Texture2D& spaceship) {
      const Rectangle src
        = {0, 0, static_cast<float>(spaceship.width), static_cast<float>(spaceship.height)};
      const Rectangle dest = {
        player.position.x, player.position.y, static_cast<float>(spaceship.width),
        static_cast<float>(spaceship.height)};
      const auto center = Vector2 {spaceship.width / 2.f, spaceship.height / 2.f};
      const auto rotation_deg = player.rotation * 180.f / 3.14f;
      DrawTexturePro(spaceship, src, dest, center, rotation_deg, colors::white);
      // Draw aim direction, first pointing up
      const auto sin_value = std::sin(player.rotation - 3.14f / 2.f);
      const auto cos_value = std::cos(player.rotation - 3.14f / 2.f);
      const auto aim_direction = Vector2Scale(Vector2 {cos_value, sin_value}, 50.f);
      const auto start = player.position;
      const auto end = Vector2Add(start, aim_direction);
      if (db.has_component<gun>(id)) {
        const auto& gun_info = db.get_component<gun>(id);
        std::string str = std::format("Ammo: {}", gun_info.ammo);
        int x = int(player.position.x) + 10;
        int y = int(player.position.y) + 10;
        DrawText(str.c_str(), x, y, 20, colors::white);
      }
      DrawLineV(start, end, colors::red);
    });
  }
} // namespace systems::player
