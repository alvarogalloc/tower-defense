export module systems.bullet;
import raylib;
import ginseng;
import std;

export namespace systems::bullet {
  struct info {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    std::uint8_t damage;
  };

  void shoot_bullet(ginseng::database& db, info info);

  void update(ginseng::database& db, float dt);
  void draw(ginseng::database& db);

} // namespace systems::bullet
