module ellipse_scene;
import menu_scene;
import fmt;
import raygui;
import debug;
import game_config;
import raylib_utils;
import debug_gui_system;

ellipse_scene::ellipse_scene()
    : m_target_manager(std::ifstream {SRC_DIR "/assets/targets.json"})
    , m_draw_system(m_target_manager, m_detached_bullets)
    , m_physics_system(m_target_manager, m_detached_bullets)
    , m_debug_gui_system(m_bullet_rings, m_target_manager, m_detached_bullets)
{
  namespace fs = std::filesystem;

  for (const auto rings_path = fs::path {SRC_DIR "/assets/ring_presets"};
       const auto& current_entry : fs::directory_iterator {rings_path}) {
    fmt::print(debug::info, "loading ring {}\n", current_entry.path());
    m_bullet_rings.emplace_back(bullets::info::load(current_entry.path().string()));
  }
  debug::my_assert(!m_bullet_rings.empty(), "No bullet rings loaded");
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
  if (IsKeyPressed(KEY_D)) {
    m_draw_debug_gui_detached_bullets = !m_draw_debug_gui_detached_bullets;
  }
  if (IsKeyPressed(KEY_R)) {
    std::ranges::for_each(m_bullet_rings, &bullets::respawn_dead);
  }

  // shoot
  if (IsKeyPressed(KEY_SPACE)) {
    debug::my_assert(m_selected_ring_index < m_bullet_rings.size(), "Invalid ring index");
    if (m_bullet_rings.at(m_selected_ring_index).count_alive() > 0) {
      m_detached_bullets.push_back([this] {
        const auto mouse_pos = GetMousePosition();
        auto new_bullet = m_bullet_rings.at(m_selected_ring_index).detach_bullet(mouse_pos);
        // now we get the edge of the ellipse for the target
        Vector2 target_pos {9999, 9999}; // initial should be something far away
        for (auto& target : m_target_manager.get_targets()) {
          Vector2 edge
            = m_bullet_rings.at(m_selected_ring_index).get_edge_for(target.pos, mouse_pos);
          // check if the edge is closer to the target than the current target_pos
          if (Vector2Distance(edge, target.pos) < Vector2Distance(target_pos, target.pos)) {
            target_pos = target.pos;
          }
        }
        new_bullet.target_position = target_pos;
        return new_bullet;
      }());
    }
  }
  if (IsKeyPressed(KEY_LEFT)) {
    m_selected_ring_index
      = (m_selected_ring_index + m_bullet_rings.size() - 1) % m_bullet_rings.size();
  }
  if (IsKeyPressed(KEY_RIGHT)) {
    m_selected_ring_index = (m_selected_ring_index + 1) % m_bullet_rings.size();
  }

  /*
  */
  // spawn target if shift+click
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)) {
    const target new_target {
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
    auto removed_targets
      = std::ranges::remove_if(m_target_manager.get_targets(), [](const auto& target) {
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
  return std::make_unique<menu_scene>();
}

void ellipse_scene::on_render()
{
  // galaxy blue
  ClearBackground(get_game_config().bg_color);

  const auto mouse_pos = GetMousePosition();
  for (auto ring_index : std::views::iota(0uz, m_bullet_rings.size())) {
    m_bullet_rings.at(ring_index).draw(mouse_pos, m_selected_ring_index == ring_index);
  }

  m_draw_system.draw();

  const auto score_str = fmt::format("Score: {}", m_score);
  constexpr static Vector2 score_pos = {10.f, 10.f};
  DrawText(score_str.c_str(), int(score_pos.x), int(score_pos.y), 20, colors::white);
  if (m_draw_debug_gui) {
    m_debug_gui_system.draw(m_selected_ring_index);
  }

  if (m_draw_debug_gui_detached_bullets) {
    m_debug_gui_system.draw_list_detached_bullets();
  }
}
