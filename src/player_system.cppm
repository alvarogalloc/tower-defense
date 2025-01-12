export module player_system;
import raylib;
import std;
export struct player_movement {
  Vector2 position;
  Vector2 velocity;
  float acceleration_rate;
  float deceleration_rate;
  float max_speed;
  float rotation;
};

export enum class player_action : std::uint8_t {
  none,
  shoot,
  reload,
  change_forward,
  change_backward,
};

export
{
  std::function<player_action(float)> make_player_system(player_movement & player)
  {
    return [&](float dt) {
      Vector2 thrust = {0.0f, 0.0f};
      constexpr static float rotation_factor = 3.14f / 100.f;
      constexpr static float acceleration_factor = 1.0f;
      const float sin_value = std::sin(player.rotation);
      const float cos_value = std::cos(player.rotation);
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

      if (thrust.x != 0.0f || thrust.y != 0.0f) {
        thrust = Vector2Scale(
          Vector2Normalize(thrust), player.acceleration_rate * dt); // Apply acceleration
        player.velocity = Vector2Add(player.velocity, thrust);

        if (Vector2Length(player.velocity) > player.max_speed) {
          player.velocity
            = Vector2Scale(Vector2Normalize(player.velocity), player.max_speed); // Clamp velocity
        }
      } else {
        // Decelerate when no thrust is applied
        Vector2 deceleration
          = Vector2Scale(Vector2Normalize(player.velocity), -player.deceleration_rate * dt);
        player.velocity = Vector2Add(player.velocity, deceleration);

        // Stop if velocity is very low
        if (Vector2Length(player.velocity) < 10.0f) {
          player.velocity = {0.0f, 0.0f};
        }
      }

      // Update position
      player.position = Vector2Add(player.position, Vector2Scale(player.velocity, dt));
      return player_action::none;
    };
  }
}
