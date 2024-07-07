export module utils;
import stdbridge;
import fmt;
import sfml;
import json;
import tmx;

export
{

  template<typename T>
  concept with_bounds =
    std::convertible_to<T, sf::Transformable> && requires(T t) {
      { t.getGlobalBounds() };
      { t.getLocalBounds() };
    };

  template<typename T>
  concept vector_2 = requires(T t) {
    { t.x };
    { t.y };
  };

  constexpr auto warn = fmt::emphasis::bold | fmt::fg(fmt::color::yellow);
  constexpr auto error = fmt::emphasis::bold | fmt::fg(fmt::color::red);
  constexpr auto info = fmt::emphasis::bold | fmt::fg(fmt::color::cyan);
  constexpr auto success = fmt::emphasis::bold | fmt::fg(fmt::color::green);

  std::string get_resource_path();


  template<vector_2 T> inline auto normalize(T vec)
  {
    float inside_root = vec.x * vec.x + vec.y * vec.y;
    float magnitude = std::sqrt(inside_root);
    return T{ vec.x / magnitude, vec.y / magnitude };
  };


  auto sprite_center(const with_bounds auto &sprite)
  {
    return sf::Vector2f{ sprite.getGlobalBounds().width / 2,
      sprite.getGlobalBounds().height / 2 };
  }
  void flip_sprite(sf::Transformable & target, bool flip_x, bool flip_y);


  std::vector<sf::IntRect> load_animation_frames_json(
    const nlohmann::json &parent_node, std::string_view name);


  void my_assert(bool condition,
    std::string_view message = "",
    std::source_location loc = std::source_location::current());


  std::vector<sf::Vector2f> get_points(
    tmx_layer *const layer_head, std::string_view layer_name);

  template<vector_2 T> constexpr auto to_string(const T &v)
  {
    return fmt::format("(x: {}, y: {})", v.x, v.y);
  }


  template<typename T> constexpr auto to_string(const sf::Rect<T> &r)
  {
    return fmt::format("left: {}, top: {}, width: {}, height: {}",
      r.left,
      r.top,
      r.width,
      r.height);
  }


  constexpr auto to_string(const sf::Sprite &sprite)
  {
    return fmt::format("sprite: position: {}, texture rect: {}",
      to_string(sprite.getPosition()),
      to_string(sprite.getTextureRect()));
  }
}
