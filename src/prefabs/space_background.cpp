module prefabs.space_background;
import config;
import std;
import debug;


namespace prefabs::v2
{
namespace
{
// constexpr float background_scroll_factor = 0.5f;
inline constexpr int object_spawn_offset_min = 100;
inline constexpr int object_spawn_offset_max = 100;
// constexpr int object_vertical_offset = 100;
} // namespace
space_background::space_background(
    std::string_view background_texture_path,
    std::vector<std::string> object_texture_paths, const float scroll_speed)
    : m_background(LoadTexture(background_texture_path.data())),
      m_object_data(), m_scroll_speed(scroll_speed)
{
    debug::my_assert(std::filesystem::exists(background_texture_path),
                     "File does not exist: " +
                         std::string(background_texture_path));
    auto generate_start_pos = [] {
        return Vector2{
            static_cast<float>(
                GetRandomValue(object_spawn_offset_min,
                               GetScreenWidth() - object_spawn_offset_max)),
            static_cast<float>(
                GetRandomValue(object_spawn_offset_min,
                               GetScreenHeight() - object_spawn_offset_max)),
        };
    };

    for (const auto &path : object_texture_paths)
    {
        debug::my_assert(std::filesystem::exists(path),
                         "File does not exist: " + std::string(path));
        m_object_data.emplace_back(LoadTexture(path.data()),
                                   generate_start_pos());
    }
}

space_background::~space_background()
{
    UnloadTexture(m_background);
    for (const auto &[texture, _] : m_object_data)
    {
        UnloadTexture(texture);
    }
}

void space_background::update()
{
  for (auto &[texture, position] : m_object_data)
  {
    position.x -= m_scroll_speed;
    if (position.x < -float(texture.width))
    {
      // reset_object_position(i++);
      position.x = float(GetScreenWidth());
    }
  }
}

void space_background::draw() const
{
    // DrawTexturePro(m_background, 0, 0, colors::white);
    DrawTexturePro(m_background, {0, 0, float(m_background.width), float(m_background.height)}, {0, 0, float(GetScreenWidth()), float(GetScreenHeight())}, {0,0}, 0, colors::white);



    for (const auto &[texture, position] : m_object_data)
    {
        DrawTextureEx(texture, position, 0.f, 1.f, colors::white);
    }
}
} // namespace prefabs::v2

