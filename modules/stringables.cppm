export module stringables;
import stdutils;
import fmt;
import sfml;

export
{

    template <typename T> constexpr auto to_string(const sf::Vector2<T> &v)
    {
        return fmt::format("(x: {}, y: {})", v.x, v.y);
    }

    template <typename T> constexpr auto to_string(const sf::Rect<T> &r)
    {
        return fmt::format("left: {}, top: {}, width: {}, height: {}", r.left, r.top, r.width, r.height);
    }

    constexpr auto to_string(const sf::Sprite &sprite)
    {
        return fmt::format("sprite: position: {}, texture rect: {}", to_string(sprite.getPosition()),
                           to_string(sprite.getTextureRect()));
    }
}
