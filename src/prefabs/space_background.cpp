module prefabs.space_background;
import game;
import std;
import debug;
import utils.assets_cache;
using namespace rooster;

namespace prefabs::v2 {
space_background::space_background(
    std::string_view background_texture_path,
    std::vector<std::string> const &object_texture_paths,
    const float scroll_speed)
    : m_background(utils::get_asset<utils::asset_type::texture>(background_texture_path)),
      m_object_data(),
      m_scroll_speed(scroll_speed) {
  auto [w, h] = game::get().get_config().get_app_info().game_res;
  auto generate_start_pos = [&] {
    return Vector2{
        float(GetRandomValue(0, int(w))),
        float(GetRandomValue(0, int(h))),
    };
  };
  auto gen_scale = [] { return float(GetRandomValue(1, 15)) / 10.f; };
  const int amount_per_texture = 2;

  for (const auto &path : object_texture_paths) {
    auto tex = utils::get_asset<utils::asset_type::texture>(path);
    for (int _ : std::views::iota(0, amount_per_texture)) {
      m_object_data.emplace_back(tex, generate_start_pos(), gen_scale());
      m_object_data.emplace_back(tex, generate_start_pos(), gen_scale());
      m_object_data.emplace_back(tex, generate_start_pos(), gen_scale());
    }
  }
}

space_background::space_background(config::space_bg const &config_node)
    : space_background(
          config_node.bg_texture,
          config_node.objects_textures, config_node.scroll_speed) {}


void space_background::update() {
  auto [w, _] = game::get().get_config().get_app_info().game_res;
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
  auto [w, h] = game::get().get_config().get_app_info().game_res;
  DrawTexturePro(m_background,
                 {0, 0, float(m_background.width), float(m_background.height)},
                 {0, 0, w, h}, {0, 0}, 0, colors::white);

  for (const auto &[texture, position, scale] : m_object_data) {
    DrawTextureEx(texture, position, 0.f, scale, colors::white);
  }
}
}  // namespace prefabs::v2
