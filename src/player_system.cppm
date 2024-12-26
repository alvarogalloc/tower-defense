export module player_system;
import raylib;
import std;
export struct player_movement {
  Vector2 position;
  Vector2 velocity;
  float acceleration_rate;
  float deceleration_rate;
  float max_speed;
};

export std::function<void(float)> make_player_system(player_movement& player)
{
  return [&](float dt) {
    Vector2 thrust = {0.0f, 0.0f};

    if (IsKeyDown(KEY_W)) thrust.y -= 1.0f; // Up
    if (IsKeyDown(KEY_S)) thrust.y += 1.0f; // Down
    if (IsKeyDown(KEY_A)) thrust.x -= 1.0f; // Left
    if (IsKeyDown(KEY_D)) thrust.x += 1.0f; // Right

    if (thrust.x != 0.0f || thrust.y != 0.0f) {
      thrust = Vector2Scale(Vector2Normalize(thrust), player.acceleration_rate * dt); // Apply acceleration
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
  };
}
