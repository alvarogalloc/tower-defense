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
    for (const auto& current : data) {
      datastr += fmt::format("{}{}: {};", name, i, transformer(current));
      i++;
    }
    datastr.pop_back(); // remove last ';'
    return datastr;
  }

  /*inline void poll_collision(bullets& ring, bullet& current_bullet) {}*/

} // namespace

ellipse_scene::ellipse_scene()
{
  namespace fs = std::filesystem;

  for (const auto rings_path = fs::path {SRC_DIR "/assets/ring_presets"};
       const auto& current_entry : fs::directory_iterator {rings_path}) {
    fmt::print(info, "loading ring {}\n", current_entry.path());
    m_bullet_rings.emplace_back(bullet_group_info::load(current_entry.path().string()));
  }

  m_targets.emplace_back(Vector2 {100, 100}, 10.f, colors::red, 100.f, 100.f);
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
    std::ranges::for_each(m_bullet_rings, [](auto& ring) {
      ring.respawn_dead();
    });
  }

  if (IsKeyPressed(KEY_SPACE)) {
    if (m_current_ring == std::end(m_bullet_rings) || m_current_ring->count_alive() == 0) {
      m_current_ring = std::ranges::find_if(m_bullet_rings, [](const auto& b_ring) {
        return b_ring.count_alive() > 0;
      });
    }
    // TODO change 'm_bullet_rings.front()' to a selected ring, or maybe all
    if (m_current_ring != std::end(m_bullet_rings)) {
      auto m_pos = GetMousePosition();
      auto& new_detached_bullet
        = m_detached_bullets.emplace_back(m_current_ring->detach_bullet(m_pos));

      // set the target to the closest target
      new_detached_bullet.target = m_closest_target->pos;
    }
  }

  // spawn target if shift+click
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)) {
    m_targets.emplace_back(GetMousePosition(), 10.f, colors::red, 100.f, 100.f);
  }

  std::ranges::for_each(m_bullet_rings, [frame_time = GetFrameTime()](auto& ring) {
    ring.update(frame_time);
    /*std::ranges::for_each(m_de)*/
    /**/
    /*poll_collision(bullets &ring, bullet &current_bullet)*/
  });

  // get the closest distance bullet to the mouse
  m_closest_target = std::ranges::min_element(
    m_targets, [pos = GetMousePosition()](const auto& lhs, const auto& rhs) {
      return std::hypot(lhs.pos.x - pos.x, lhs.pos.y - pos.y)
        < std::hypot(rhs.pos.x - pos.x, rhs.pos.y - pos.y);
    });

  // if detached bullet has been fired
  std::ranges::for_each(m_detached_bullets, [&](auto& current_bullet) {
    if (!current_bullet.valid()) return;
    // if out of bounds, dont update it again
    // if (
    //   m_detached_bullet.position.x < 0 || m_detached_bullet.position.x > float(GetScreenWidth())
    //   || m_detached_bullet.position.y < 0
    //   || m_detached_bullet.position.y > float(GetScreenHeight())) {
    //   m_detached_bullet.info = nullptr;
    // }
    // else {
    // point to the closest target

    auto direction = current_bullet.target - current_bullet.position;
    current_bullet.velocity = current_bullet.velocity + direction * dt;
    current_bullet.position = current_bullet.position + current_bullet.velocity * dt;
  });
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
