module prefabs.space_background;
import game;
import std;
import debug;
using namespace rooster;

namespace prefabs::v2
{
namespace
{
// constexpr float background_scroll_factor = 0.5f;
inline constexpr int object_spawn_offset_min = 100;
inline constexpr int object_spawn_offset_max = 100;
// constexpr int object_vertical_offset = 100;
} // namespace
space_background::space_background(std::string_view background_texture_path,
                                   rapidjson::Value const &object_texture_paths,
                                   const float scroll_speed)
    : m_background(LoadTexture(background_texture_path.data())),
      m_object_data(), m_scroll_speed(scroll_speed)
{
    auto [w, h] = game::get().get_spec().game_res;
    debug::my_assert(
        std::filesystem::exists(background_texture_path),
        std::format("File does not exist: {}", background_texture_path));
    auto generate_start_pos = [&] {
        return Vector2{
            float(GetRandomValue(object_spawn_offset_min,
                                 int(w) - object_spawn_offset_max)),
            float(GetRandomValue(object_spawn_offset_min,
                                 int(h) - object_spawn_offset_max)),
        };
    };

    for (const auto &path : object_texture_paths.GetArray())
    {
        auto str_path = std::format("{}/assets/{}", SRC_DIR, path.GetString());
        debug::my_assert(std::filesystem::exists(str_path),
                         std::format("File does not exist: {} ", str_path));
        m_object_data.emplace_back(LoadTexture(str_path.c_str()),
                                   generate_start_pos());
    }
}

space_background::space_background(rapidjson::Value const &config_node)
    : space_background(std::format("{}/assets/{}", SRC_DIR,
                                   config_node["bg_texture"].GetString()),
                       config_node["objects_textures"].GetArray(),
                       config_node["scroll_speed"].GetFloat())
{
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
    auto [w, _] = game::get().get_spec().game_res;
    for (auto &[texture, position] : m_object_data)
    {
        position.x -= m_scroll_speed;
        if (position.x < -float(texture.width))
        {
            // reset_object_position(i++);
            position.x = float(w);
        }
    }
}

void space_background::draw() const
{
    // DrawTexturePro(m_background, 0, 0, colors::white);
    auto [w, h] = game::get().get_spec().game_res;
    DrawTexturePro(
        m_background,
        {0, 0, float(m_background.width), float(m_background.height)},
        {0, 0, w, h}, {0, 0}, 0, colors::white);

    for (const auto &[texture, position] : m_object_data)
    {
        DrawTextureEx(texture, position, 0.f, 1.f, colors::white);
    }
}
} // namespace prefabs::v2
