module;
#ifdef __APPLE__
#include <CoreFoundation/CFBundle.h>
#endif
module utils;


std::string get_resource_path()
{
#ifdef __APPLE__
  auto bundle = CFBundleGetMainBundle();
  auto *url = CFBundleCopyBundleURL(bundle);
  auto *path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
  auto *extension = CFURLCopyPathExtension(url);
  std::string path_str =
    CFStringGetCStringPtr(path, kCFStringEncodingUTF8) + std::string{ "/" };
  if (extension != nullptr) { path_str += "Contents/Resources/"; }


  // CFRelease(url);
  // CFRelease(path);
  return path_str;
#else
#error "not supported platform"
#endif
}


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

    auto msg = fmt::format(
      "assertion failed: {} (at {}:{})", message, loc.file_name(), loc.line());
    throw std::runtime_error(msg);
  }
}


std::vector<sf::Vector2f> get_points(tmx_layer *layer_head,
  std::string_view layer_name)
{
  my_assert(layer_head != nullptr, "no layers found");
  while (layer_head != nullptr
         && !std::string(layer_name).starts_with(layer_head->name))
  {

    layer_head = layer_head->next;
  }

  my_assert(layer_head != nullptr, " invalid layer found");

  std::vector<sf::Vector2f> points;
  auto current_object = layer_head->content.objgr->head;
  while (current_object)
  {
    points.push_back({ sf::Vector2f{ static_cast<float>(current_object->x),
      static_cast<float>(current_object->y) } });
    current_object = current_object->next;
  }
  std::reverse(points.begin(), points.end());
  return points;
}


void flip_sprite(sf::Transformable &target, bool flip_x, bool flip_y)
{
  auto g = target.getScale();
  g.x = std::abs(g.x);
  g.y = std::abs(g.y);
  target.setScale(flip_x ? -g.x : g.x, flip_y ? -g.y : g.y);
}
