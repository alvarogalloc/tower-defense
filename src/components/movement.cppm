export module components.movement;
import raylib;

export namespace components {

  struct movement {
    Vector2 position;
    Vector2 velocity;
    float acceleration_rate;
    float deceleration_rate;
    float max_speed;
    float rotation;
  };

}
