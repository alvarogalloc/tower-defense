module bullets;
import std;
import debug;
import fmt;
import raygui;
//
// the bullets will be circling in three ellipses around the player
// in angles 0, 120, 240 (in radians: 0, 2pi/3, 4pi/3)

namespace {
  constexpr auto pi = std::numbers::pi_v<float>;
  constexpr float bullet_radius {3.f};
} // namespace

bullets::bullets(const bullet_group_info& info)
    : m_info {info}
{
  m_bullets.resize(m_info.count);
  std::ranges::fill(m_bullets, bullet {.alive = true, .acc_time = 0.f});
  respawn_dead();
}

void bullets::draw(const Vector2 center) const
{
  // // draw the bullets
  std::string entities_list;
  for (const auto& b : m_bullets) {
    if (!b.alive) {
      continue;
    }
    entities_list += fmt::format("bullet: {}\n", b.acc_time);
    const Vector2 pos {
      std::cos(b.acc_time) * m_info.ellipse_radius.x + center.x,
      std::sin(b.acc_time) * m_info.ellipse_radius.y + center.y};
    DrawCircleV(pos, bullet_radius, m_info.color);
  }
}

void bullets::respawn_dead() {
  std::ranges::for_each(m_bullets, [](auto& b) {
    const auto random_angle = float(GetRandomValue(0, 360));
    b.acc_time = (random_angle * pi) / 180.f;
  });
}

detached_bullet bullets::detach_bullet(const Vector2 center)
{
  detached_bullet detached;

  auto res = std::find_if(m_bullets.begin(), m_bullets.end(), [](const auto& b) {
    return b.alive;
  });
  if (res != m_bullets.end()) {
    const Vector2 pos {
      std::cos(res->acc_time) * m_info.ellipse_radius.x + center.x,
      std::sin(res->acc_time) * m_info.ellipse_radius.y + center.y};

    const Vector2 vel {
      std::cos(res->acc_time) * m_info.speed,
      std::sin(res->acc_time) * m_info.speed};

    detached.position = pos;
    detached.velocity = vel;
    detached.info = &m_info;
    res->alive = false;
  }
  return detached;
}

void bullets::update(const float delta)
{
  // only one ellipse now

  for (auto& b : m_bullets | std::views::filter([](const auto& b) {
                   return b.alive;
                 })) {
    b.acc_time += delta * m_info.speed;
  }
}
