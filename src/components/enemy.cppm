export module components.enemy;
import std;
import raylib;

export namespace components {
  struct enemy {
    enum class type {
      basic,
      fast,
      strong,
    };
    type enemy_type;
    [[nodiscard]] std::string to_string() const {
      std::string enemy_type_str;
      switch (enemy_type) {
        case type::basic: enemy_type_str = "basic"; break;
        case type::fast: enemy_type_str = "fast"; break;
        case type::strong: enemy_type_str = "strong"; break;
      }
      return std::format 
        ("enemy_type: {}, health: {}, damage: {}, speed: {}", enemy_type_str, health, damage, speed);
    }
    std::uint8_t health;
    std::uint8_t damage;
    Texture2D texture;
    float speed;
  };
} // namespace components
