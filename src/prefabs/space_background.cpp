module prefabs;
import std;
import wey;
using namespace rooster;

namespace prefabs {

space_background::space_background(const json_data data, context_view ctx)
    : m_background(ctx.assets.get<asset_type::texture>(data.bg_texture_path)),
      m_object_data(),
      m_scroll_speed(data.scroll_speed),
      m_context(ctx) {
  auto [w, h] = m_context.app_info.game_res;
  auto generate_start_pos = [&] {
    return Vector2{
        float(GetRandomValue(0, int(w))),
        float(GetRandomValue(0, int(h))),
    };
  };
  const auto gen_scale = [] { return float(GetRandomValue(1, 15)) / 10.f; };
  const int amount_per_texture = 2;

  for (const auto &path : data.objects_textures_paths) {
    auto tex = ctx.assets.get<asset_type::texture>(path);
    for (int _ : std::views::iota(0, amount_per_texture)) {
      m_object_data.emplace_back(tex, generate_start_pos(), gen_scale());
      m_object_data.emplace_back(tex, generate_start_pos(), gen_scale());
      m_object_data.emplace_back(tex, generate_start_pos(), gen_scale());
    }
  }
}

void space_background::update() {
  auto [w, _] = m_context.app_info.game_res;

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
  auto [w, h] = m_context.app_info.game_res;
  DrawTexturePro(m_background,
                 {0, 0, float(m_background.width), float(m_background.height)},
                 {0, 0, w, h}, {0, 0}, 0, colors::white);

  for (const auto &[texture, position, scale] : m_object_data) {
    DrawTextureEx(texture, position, 0.f, scale, colors::white);
  }
}
}  // namespace prefabs
