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
  std::function<player_action(float)> make_player_system(player_movement & player);
  std::function<void()> make_draw_player(player_movement & player, Texture2D & spaceship);
}
