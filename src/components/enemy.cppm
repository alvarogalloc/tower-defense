export module components.enemy;
import std;
import raylib;

export namespace components {
struct enemy {
  std::uint8_t health;
  std::uint8_t max_health;
  std::uint8_t damage;
  std::string texture_path;
  float speed;
};

}  // namespace components
