module;
#ifdef __APPLE__
#include <CoreFoundation/CFBundle.h>
#endif
export module utils;
import stdutils;
import fmt;
import sfml;
import tmx;
import json;

export
{
    auto get_resource_path()
    {
#ifdef __APPLE__
        auto bundle = CFBundleGetMainBundle();
        auto *url = CFBundleCopyBundleURL(bundle);
        auto *path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
        auto *extension = CFURLCopyPathExtension(url);
        std::string path_str =
          CFStringGetCStringPtr(path, kCFStringEncodingUTF8)
          + std::string{ "/" };
        if (extension != nullptr) { path_str += "Contents/Resources/"; }


        // CFRelease(url);
        // CFRelease(path);
        return path_str;
#else
#error "not supported platform"
#endif
    }

    template<typename T>
    concept with_bounds =
      std::convertible_to<T, sf::Transformable> && requires(T t) {
          { t.getGlobalBounds() } -> std::convertible_to<sf::FloatRect>;
          { t.getLocalBounds() } -> std::convertible_to<sf::FloatRect>;
      };

    inline sf::Vector2f normalize(sf::Vector2f vec)
    {
        float inside_root = vec.x * vec.x + vec.y * vec.y;
        float magnitude = std::sqrt(inside_root);
        return sf::Vector2f{ vec.x / magnitude, vec.y / magnitude };
    };


    sf::Vector2f sprite_center(const with_bounds auto &sprite)
    {
        return sf::Vector2f{ sprite.getGlobalBounds().width / 2,
            sprite.getGlobalBounds().height / 2 };
    }

    std::vector<sf::IntRect> load_animation_frames_json(
      const nlohmann::json &parent_node, std::string_view name);

    void my_assert(bool condition,
      std::string_view message = "",
      std::source_location loc = std::source_location::current());


    constexpr std::vector<sf::Vector2f> get_turning_points(
      tmx_layer * layer_head)
    {
        my_assert(layer_head != nullptr, "no layers found");
        while (layer_head)
        {
            if (layer_head->type == L_OBJGR) { break; }
            layer_head = layer_head->next;
        }
        my_assert(layer_head != nullptr, "no object layer found");

        std::vector<sf::Vector2f> points;
        auto current_object = layer_head->content.objgr->head;
        while (current_object)
        {
            points.push_back(
              { sf::Vector2f{ static_cast<float>(current_object->x),
                static_cast<float>(current_object->y) } });
            current_object = current_object->next;
        }
        std::reverse(points.begin(), points.end());
        return points;
    }


    template<typename T> constexpr auto to_string(const sf::Vector2<T> &v)
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

module :private;

std::vector<sf::IntRect> load_animation_frames_json(
  const nlohmann::json &parent_node,
  std::string_view name)
{
    my_assert(parent_node.contains(name),
      fmt::format("no suggested key \"{}\" found in the json", name));
    my_assert(parent_node[name].is_array(), "frames must be an array");
    std::vector<sf::IntRect> frames;
    for (auto &&rect : parent_node[name])
    {
        frames.push_back({ rect[0], rect[1], rect[2], rect[3] });
    }
    return frames;
}

void my_assert(bool condition,
  std::string_view message,
  std::source_location loc)
{
    if (!condition)
    {

        auto msg = fmt::format("assertion failed: {} (at {}:{})",
          message,
          loc.file_name(),
          loc.line());
        throw std::runtime_error(msg);
    }
}
