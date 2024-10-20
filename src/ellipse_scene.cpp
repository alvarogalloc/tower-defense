module ellipse_scene;
import menu_scene;
import fmt;
import raygui;
import debug;
import game_config;

namespace {
  constexpr auto operator+(const Vector2& lhs, const Vector2& rhs)
  {
    return Vector2 {lhs.x + rhs.x, lhs.y + rhs.y};
  }
  constexpr auto operator-(const Vector2& lhs, const Vector2& rhs)
  {
    return Vector2 {lhs.x - rhs.x, lhs.y - rhs.y};
  }
  constexpr auto operator*(const Vector2& lhs, float rhs)
  {
    return Vector2 {lhs.x * rhs, lhs.y * rhs};
  }
  // constexpr auto operator/(const Vector2& lhs, float rhs)
  // {
  //   return Vector2 {lhs.x / rhs, lhs.y / rhs};
  // }

  constexpr auto to_string(const Vector2& v)
  {
    return fmt::format("({:.2f},{:.2f})", v.x, v.y);
  }

  constexpr auto list_to_datastr(std::string name, std::ranges::range auto data, auto transformer)
  {
    std::string datastr;
    std::size_t i = 0;
    for (auto& current : data) {
      datastr += fmt::format("{}{}: {};", name, i, transformer(current));
      i++;
    }
    datastr.pop_back(); // remove last ';'
    return datastr;
  }

} // namespace

ellipse_scene::ellipse_scene()
{
  namespace fs = std::filesystem;

  for (const auto rings_path = fs::path {SRC_DIR "/assets/ring_presets"};
       const auto& current_entry : fs::directory_iterator {rings_path}) {
    fmt::print(debug::info, "loading ring {}\n", current_entry.path());
    m_bullet_rings.emplace_back(bullet_group_info::load(current_entry.path().string()));
  }

  m_targets.emplace_back(Vector2 {100, 100}, 10.f, colors::red, 10.f, 100.f);
  m_targets.emplace_back(Vector2 {400, 100}, 20.f, colors::red, 100.f, 100.f);
  m_current_ring = m_bullet_rings.begin();
}

void ellipse_scene::on_start() {}

void ellipse_scene::on_update()
{
  const float dt = GetFrameTime();
  if (IsKeyPressed(KEY_ENTER)) {
    m_should_exit = true;
  }
  if (IsKeyPressed(KEY_TAB)) {
    m_draw_debug_gui = !m_draw_debug_gui;
  }
  if (IsKeyPressed(KEY_R)) {
    // there's no range alternative, so we use accumulate
    auto sum = std::accumulate(
      m_bullet_rings.begin(), m_bullet_rings.end(), 0, [](auto acc, const auto& ring) {
        return acc + ring.count_dead();
      });
    fmt::print("respawning {} bullets\n", sum);
    std::ranges::for_each(m_bullet_rings, &bullets::respawn_dead);
  }

  // get the closest distance bullet to the mouse
  m_closest_target = std::ranges::min_element(
    m_targets, [pos = GetMousePosition()](const auto& lhs, const auto& rhs) {
      return std::hypot(lhs.pos.x - pos.x, lhs.pos.y - pos.y)
        < std::hypot(rhs.pos.x - pos.x, rhs.pos.y - pos.y);
    });
  if (IsKeyPressed(KEY_SPACE)) {
    if (m_current_ring == std::end(m_bullet_rings) || m_current_ring->count_alive() == 0) {
      m_current_ring = std::ranges::find_if(m_bullet_rings, [](const auto& b_ring) {
        return b_ring.count_alive() > 0;
      });
    }
    // TODO change 'm_bullet_rings.front()' to a selected ring, or maybe all
    if (m_current_ring != std::end(m_bullet_rings)) {
      auto m_pos = GetMousePosition();
      m_detached_bullets.emplace_back(m_current_ring->detach_bullet(m_pos));
    }
  }

  // spawn target if shift+click
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)) {
    m_targets.emplace_back(GetMousePosition(), 10.f, colors::red, 100.f, 100.f);
  }

  for (auto& ring : m_bullet_rings) {
    ring.update(dt);
  }

  for (auto& target : m_targets) {
    auto [begin, end] = std::ranges::remove_if(m_detached_bullets, [&](auto& bullet) {
      if (!bullet.valid()) return true;
      return std::hypot(target.pos.x - bullet.position.x, target.pos.y - bullet.position.y)
        < target.radius;
    });
    for (auto it = begin; it != end; ++it) {
      target.health -= it->info->damage;
    }
    m_detached_bullets.erase(begin, end);
  }

  // if detached bullet has been fired
  for (auto& current_bullet : m_detached_bullets) {
    if (!current_bullet.valid()) continue; // Skip invalid bullets

    fmt::print(debug::info, "bullet: {}\n", to_string(current_bullet.position));

    // Calculate direction to target
    auto direction = m_closest_target->pos - current_bullet.position;
    auto direction_normalized = Vector2Normalize(direction); // Normalize direction vector

    constexpr static float max_speed = 200.f;
    constexpr static float acceleration = 500.f; // Adjust acceleration value as needed

    // Calculate new velocity with acceleration
    current_bullet.velocity = current_bullet.velocity + direction_normalized * acceleration * dt;

    // Limit velocity to max speed
    if (std::hypot(current_bullet.velocity.x, current_bullet.velocity.y) > max_speed) {
      current_bullet.velocity = Vector2Normalize(current_bullet.velocity) * max_speed;
    }

    // Update position
    current_bullet.position = current_bullet.position + current_bullet.velocity * dt;
  }
  // remove dead targets, but update score
  auto remove_dead_targets = [&] {
    auto removed_targets = std::ranges::remove_if(m_targets, [](const auto& target) {
      return target.health <= 0;
    });

    for (const auto& target : removed_targets) {
      m_score += static_cast<std::size_t>(target.max_health);
    }
    m_targets.erase(removed_targets.begin(), removed_targets.end());
  };
  remove_dead_targets();
}

std::unique_ptr<scene> ellipse_scene::on_exit()
{
  return std::make_unique<ellipse_scene>();
}

void ellipse_scene::draw_debug_gui() const
{
  auto list = "Bullet Rings;Current Ring;Targets;Closest Target;Detached Bullets";

  static int scrollIndex = 0;
  static int active = 0;
  GuiListView({0, 0, 200, 200}, list, &scrollIndex, &active);
  // print the selected tab
  auto draw_ring_info = [](const auto& ring) {
    auto [damage, radius, speed, color, count, ellipse_radius] = ring.get_info();
    // Separator
    // make what i made but in a string separated by '\n'
    std::string info = fmt::format(
      "Damage: {}\nRadius: {}\nSpeed: {}\nColor: {:x}\nCount: {}\nRadius: {}", damage, radius,
      speed, static_cast<std::uint32_t>(ColorToInt(color)), count, to_string(ellipse_radius));
    // draw rectangle with color
    DrawRectangle(
      400, 0, 200, 200,
      GetColor(static_cast<std::uint32_t>(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))));
    GuiLabel({410, 0, 190, 200}, info.c_str());
  };
  auto draw_target_info = [](const auto& target) {
    auto [pos, radius, color, health, max_health] = target;
    std::string info = fmt::format(
      "Position: {}\nRadius: {}\nColor: {:x}\nHealth: {}\nMax Health: {}", to_string(pos), radius,
      static_cast<std::uint32_t>(ColorToInt(color)), health, max_health);
    DrawRectangle(
      400, 0, 200, 200,
      GetColor(static_cast<std::uint32_t>(GuiGetStyle(DEFAULT, BACKGROUND_COLOR))));
    GuiLabel({410, 0, 190, 200}, info.c_str());
  };
  switch (active) {
    case 0: {
      if (m_bullet_rings.empty()) {
        break;
      }
      static int ring_scroll = 0;
      static int ring_active = 0;
      auto list_str = list_to_datastr("", m_bullet_rings, [&](const auto& ring) {
        return fmt::format(
          "Ring {:x}", static_cast<std::uint32_t>(ColorToInt(ring.get_info().color)));
      });
      GuiListView({200, 0, 200, 200}, list_str.c_str(), &ring_scroll, &ring_active);
      draw_ring_info(m_bullet_rings.at(std::size_t(ring_active)));
      break;
    }
    case 1: {
      if (m_current_ring == m_bullet_rings.end()) {
        break;
      }
      draw_ring_info(*m_current_ring);
      break;
    }
    case 2: {
      if (m_targets.empty()) {
        break;
      }
      static int target_scroll = 0;
      static int target_active = 0;
      auto list_str = list_to_datastr("", m_targets, [&](const auto& target) {
        return fmt::format("Target {:x}", static_cast<std::uint32_t>(ColorToInt(target.color)));
      });
      GuiListView({200, 0, 200, 200}, list_str.c_str(), &target_scroll, &target_active);
      draw_target_info(m_targets.at(std::size_t(target_active)));
      break;
    }
    case 3: {
      if (m_closest_target == m_targets.end()) {
        break;
      }
      draw_target_info(*m_closest_target);
      break;
    }
    default: break;
  }
}

void ellipse_scene::on_render()
{
  // galaxy blue
  ClearBackground(get_game_config().bg_color);
  std::ranges::for_each(m_bullet_rings, [mouse_pos = GetMousePosition()](auto& ring) {
    ring.draw(mouse_pos);
  });

  auto draw_detached_bullet = [](const detached_bullet& b) {
    if (b.valid()) {
      DrawCircleV(b.position, b.info->radius, b.info->color);
      // Draw velocity
      DrawLineV(b.position, b.position + b.velocity, colors::yellow);
    }
  };
  std::ranges::for_each(m_detached_bullets, draw_detached_bullet);

  constexpr int separation = 20;
  for (const auto& t : m_targets | std::views::filter([](const auto& el) {
                         return el.health > 0.f;
                       })) {
    DrawText(
      fmt::format("{}/{}", t.health, t.max_health).c_str(), static_cast<int>(t.pos.x),
      static_cast<int>(t.pos.y - t.radius) - separation, 20, t.color);
    if (t.pos.x == m_closest_target->pos.x && t.pos.y == m_closest_target->pos.y) {
      DrawCircleV(t.pos, t.radius, colors::white);
      DrawLineV(t.pos, GetMousePosition(), colors::white);
    } else {
      DrawCircleV(t.pos, t.radius, t.color);
    }
  }

  if (m_draw_debug_gui) {
    draw_debug_gui();
  }
}
