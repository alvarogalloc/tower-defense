export module components.bullet;
import std;
import raylib;
export namespace components
{
struct bullet
{
    Vector2 position;
    Vector2 velocity;
    float rotation;
    std::uint8_t damage;
    float radius;
};
} // namespace components
