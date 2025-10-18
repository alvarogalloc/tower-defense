module prefabs.space_background;
import game;
import std;
import debug;
using namespace rooster;

namespace prefabs::v2 {
namespace {}  // namespace
space_background::space_background(
    std::string_view background_texture_path,
    std::vector<std::string> const &object_texture_paths,
    const float scroll_speed)
    : m_background(LoadTexture(background_texture_path.data())),
      m_object_data(),
      m_scroll_speed(scroll_speed) {
  auto [w, h] = game::get().get_spec().game_res;
  debug::my_assert(
      std::filesystem::exists(background_texture_path),
      std::format("File does not exist: {}", background_texture_path));
  auto generate_start_pos = [&] {
    return Vector2{
        float(GetRandomValue(0, int(w))),
        float(GetRandomValue(0, int(h))),
    };
  };
  auto gen_scale = [] { return float(GetRandomValue(1, 15)) / 10.f; };
  const int amount_per_texture = 2;

  for (const auto &path : object_texture_paths) {
    auto str_path = std::format("{}/assets/{}", SRC_DIR, path);
    debug::my_assert(std::filesystem::exists(str_path),
                     std::format("File does not exist: {} ", str_path));
    auto tex = LoadTexture(str_path.c_str());
    for (int _ : std::views::iota(0, amount_per_texture)) {
      m_object_data.emplace_back(tex, generate_start_pos(), gen_scale());
      m_object_data.emplace_back(tex, generate_start_pos(), gen_scale());
      m_object_data.emplace_back(tex, generate_start_pos(), gen_scale());
    }
  }
}

space_background::space_background(config::space_bg const &config_node)
    : space_background(
          std::format("{}/assets/{}", SRC_DIR, config_node.bg_texture),
          config_node.objects_textures, config_node.scroll_speed) {}

space_background::~space_background() {
  UnloadTexture(m_background);
  for (const auto &[texture, _, _] : m_object_data) {
    UnloadTexture(texture);
  }
}

void space_background::update() {
  auto [w, _] = game::get().get_spec().game_res;
  for (auto &[texture, position, _] : m_object_data) {
    position.x -= m_scroll_speed;
    if (position.x < -float(texture.width)) {
      // reset_object_position(i++);
      position.x = float(w);
    }
  }
}

void space_background::draw() const {
  // DrawTexturePro(m_background, 0, 0, colors::white);
  auto [w, h] = game::get().get_spec().game_res;
  DrawTexturePro(m_background,
                 {0, 0, float(m_background.width), float(m_background.height)},
                 {0, 0, w, h}, {0, 0}, 0, colors::white);

  for (const auto &[texture, position, scale] : m_object_data) {
    DrawTextureEx(texture, position, 0.f, scale, colors::white);
  }
}
}  // namespace prefabs::v2
