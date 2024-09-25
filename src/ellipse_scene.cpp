module ellipse_scene;
import menu_scene;
import fmt;
import raygui;

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
    for (const auto& current : data) {
      datastr += fmt::format("{}{}: {};", name, i, transformer(current));
      i++;
    }
    datastr.pop_back(); // remove last ';'
    return datastr;
  }

} // namespace

ellipse_scene::ellipse_scene()
{
  m_bullet_rings.emplace_back(bullet_group_info {
    .damage = 10,
    .radius = 3.f,
    .speed = 3.f,
    .color = colors::green,
    .count = 5,
    .ellipse_radius = {20, 50}});
  m_bullet_rings.emplace_back(bullet_group_info {
    .damage = 1,
    .radius = 5.f,
    .speed = 2.f,
    .color = colors::yellow,
    .count = 10,
    .ellipse_radius = {100, 50}});

  m_targets.emplace_back(Vector2 {100, 100}, 10.f, colors::red, 100.f, 100.f);
  m_targets.emplace_back(Vector2 {400, 100}, 20.f, colors::red, 100.f, 100.f);
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
    std::ranges::for_each(m_bullet_rings, [](auto& ring) {
      ring.respawn_dead();
    });
  }

  if (IsKeyPressed(KEY_SPACE)) {
    // TODO change 'm_bullet_rings.front()' to a selected ring, or maybe all
    auto m_pos = GetMousePosition();
    m_detached_bullet = m_bullet_rings.front().detach_bullet(m_pos);

    // set the target to the closest target
    m_detached_bullet.target = m_closest_target->pos;
  }

  // spawn target if shift+click
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)) {
    m_targets.emplace_back(GetMousePosition(), 10.f, colors::red, 100.f, 100.f);
  }

  std::ranges::for_each(m_bullet_rings, [frame_time = GetFrameTime()](auto& ring) {
    ring.update(frame_time);
  });

  // get the closest distance bullet to the mouse
  m_closest_target = std::ranges::min_element(
    m_targets, [pos = GetMousePosition()](const auto& lhs, const auto& rhs) {
      return std::hypot(lhs.pos.x - pos.x, lhs.pos.y - pos.y)
        < std::hypot(rhs.pos.x - pos.x, rhs.pos.y - pos.y);
    });

  // if detached bullet has been fired
  if (m_detached_bullet.valid()) {
    // if out of bounds, dont update it again
    // if (
    //   m_detached_bullet.position.x < 0 || m_detached_bullet.position.x > float(GetScreenWidth())
    //   || m_detached_bullet.position.y < 0
    //   || m_detached_bullet.position.y > float(GetScreenHeight())) {
    //   m_detached_bullet.info = nullptr;
    // }
    // else {
    // point to the closest target
    auto direction = m_detached_bullet.target - m_detached_bullet.position;

    m_detached_bullet.velocity = m_detached_bullet.velocity + direction * dt;
    m_detached_bullet.position = m_detached_bullet.position + m_detached_bullet.velocity * dt;

    // check for collision with targets TODO
    // }
  }
}

std::unique_ptr<scene> ellipse_scene::on_exit()
{
  return std::make_unique<ellipse_scene>();
}

void ellipse_scene::draw_debug_gui() const
{
  const int font_size = GuiGetStyle(DEFAULT, TEXT_SIZE);

  constexpr float spacing = 64.f;
  const auto list_width
    = static_cast<float>(MeasureText(
        fmt::format("targetx: {}\n", to_string(m_targets.front().pos)).c_str(), font_size))
    + spacing;
  const auto target_positions = list_to_datastr("target", m_targets, [](const auto& el) {
    return to_string(el.pos);
  });
  GuiListView({100, 200, list_width, 240}, target_positions.c_str(), nullptr, nullptr);

  // get all bullet acc_time
  const auto bullet_positions = list_to_datastr("Ring ", m_bullet_rings, [](const auto& ring) {
    std::string acc_times;
    int i = 0;
    for (const auto& b : ring.get()) {
      acc_times += fmt::format("acc_time {}: {:.2f};", i, b.acc_time);
      i++;
    }
    return acc_times;
  });
  GuiListView({100 + list_width, 200, list_width, 240}, bullet_positions.c_str(), nullptr, nullptr);
}

void ellipse_scene::on_render()
{
  // galaxy blue
  ClearBackground(Color {0, 0, 128, 255});
  std::ranges::for_each(m_bullet_rings, [mouse_pos = GetMousePosition()](auto& ring) {
    ring.draw(mouse_pos);
  });

  if (m_detached_bullet.valid()) {
    DrawCircleV(
      m_detached_bullet.position, m_detached_bullet.info->radius, m_detached_bullet.info->color);
    // Draw velocity
    DrawLineV(
      m_detached_bullet.position, m_detached_bullet.position + m_detached_bullet.velocity,
      colors::yellow);
  }
  constexpr int separation = 20;
  std::ranges::for_each(m_targets, [this](const auto& t) {
    DrawText(
      fmt::format("{}/{}", t.health, t.max_health).c_str(), static_cast<int>(t.pos.x),
      static_cast<int>(t.pos.y - t.radius) - separation, 20, t.color);
    if (t.pos.x == m_closest_target->pos.x && t.pos.y == m_closest_target->pos.y) {
      DrawCircleV(t.pos, t.radius, colors::white);
      DrawLineV(t.pos, GetMousePosition(), colors::white);
    } else {
      DrawCircleV(t.pos, t.radius, t.color);
    }
  });

  if (m_draw_debug_gui) {
    draw_debug_gui();
  }
}
