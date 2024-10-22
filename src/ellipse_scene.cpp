module ellipse_scene;
import menu_scene;
import fmt;
import raygui;
import debug;
import game_config;
import raylib_utils;

ellipse_scene::ellipse_scene()
    : m_target_manager(std::ifstream {SRC_DIR "/assets/targets.json"})
    , m_draw_system(m_target_manager, m_detached_bullets)
    , m_physics_system(m_target_manager, m_detached_bullets)
{
  namespace fs = std::filesystem;

  for (const auto rings_path = fs::path {SRC_DIR "/assets/ring_presets"};
       const auto& current_entry : fs::directory_iterator {rings_path}) {
    fmt::print(debug::info, "loading ring {}\n", current_entry.path());
    m_bullet_rings.emplace_back(bullet_group_info::load(current_entry.path().string()));
  }

  m_current_ring = m_bullet_rings.begin();
}

void ellipse_scene::on_start() {}

void ellipse_scene::on_update()
{
  const float dt = GetFrameTime();
  m_physics_system.update(dt);

  if (IsKeyPressed(KEY_ENTER)) {
    m_should_exit = true;
  }
  if (IsKeyPressed(KEY_TAB)) {
    m_draw_debug_gui = !m_draw_debug_gui;
  }
  if (IsKeyPressed(KEY_R)) {
    std::ranges::for_each(m_bullet_rings, &bullets::respawn_dead);
  }

  // get the closest distance bullet to the mouse
  if (IsKeyPressed(KEY_SPACE)) {
    if (m_current_ring == std::end(m_bullet_rings) || m_current_ring->count_alive() == 0) {
      m_current_ring = std::ranges::find_if(m_bullet_rings, [](const auto& b_ring) {
        return b_ring.count_alive() > 0;
      });
    }
    // TODO change 'm_bullet_rings.front()' to a selected ring, or maybe all
    if (m_current_ring != std::end(m_bullet_rings)) {
      auto m_pos = GetMousePosition();
      auto& new_bullet = m_detached_bullets.emplace_back(m_current_ring->detach_bullet(m_pos));
      new_bullet.target_position = m_target_manager.closest_to(m_pos).pos;
    }
  }
  // spawn target if shift+click
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)) {
    const target new_target{
      .pos = GetMousePosition(),
      .radius = 10.f,
      .color = colors::red,
      .health = 100.f,
      .max_health = 100.f,
    };
    m_target_manager.spawn(new_target);
  }

  for (auto& ring : m_bullet_rings) {
    ring.update(dt);
  }


  // if detached bullet has been fired
  // remove dead targets, but update score
  auto remove_dead_targets = [&] {
    auto removed_targets = std::ranges::remove_if(m_target_manager.get_targets(), [](const auto& target) {
      return target.health <= 0;
    });

    for (const auto& target : removed_targets) {
      m_score += static_cast<std::size_t>(target.max_health);
    }
    m_target_manager.get_targets().erase(removed_targets.begin(), removed_targets.end());
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
      if (m_target_manager.get_targets().empty()) {
        break;
      }
      static int target_scroll = 0;
      static int target_active = 0;
      auto list_str = list_to_datastr("", m_target_manager.get_targets(), [&](const auto& target) {
        return fmt::format("Target {:x}", static_cast<std::uint32_t>(ColorToInt(target.color)));
      });
      GuiListView({200, 0, 200, 200}, list_str.c_str(), &target_scroll, &target_active);
      draw_target_info(m_target_manager.get_targets().at(std::size_t(target_active)));
      break;
    }
    case 3: {
      if(m_target_manager.get_targets().empty()) {
        break;
      }
      draw_target_info(m_target_manager.closest_to(GetMousePosition()));
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


  if (m_draw_debug_gui) {
    draw_debug_gui();
  }
  m_draw_system.draw();
}
