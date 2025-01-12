module ellipse_scene;
import menu_scene;
import fmt;
import raygui;
import debug;
import std;
import game_config;
import raylib_utils;

/*constexpr static Vector2 cannon1_pos {52, 8};*/
/*constexpr static Vector2 cannon2_pos = {100, 8};*/

ellipse_scene::ellipse_scene()
    : m_target_manager(std::ifstream {SRC_DIR "/assets/targets.json"})
    , m_spaceship_movement {
        {0, 0},       // Initial position
        {0.0f, 0.0f}, // Initial velocity
        500.0f,       // Acceleration rate
        150.0f,       // Deceleration rate
        400.0f,       // Max speed
        0.f           // Initial rotation
      }
    ,m_update_player(make_player_system(m_spaceship_movement))

{
  constexpr Vector2 starting_pos {500, 500};

  // spawn player at the center
  m_spaceship_movement.position = Vector2Scale(starting_pos, 0.5f);
  m_spaceship = LoadTexture(SRC_DIR "/assets/spaceship.png");
}

void ellipse_scene::on_start() {}

void ellipse_scene::on_update()

{
  const float dt = GetFrameTime();
  m_update_player(dt);

  if (IsKeyPressed(KEY_ENTER)) {
    m_should_exit = true;
  }

  // spawn target if shift+click
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_SHIFT)) {
    const target new_target {
      .radius = 10.f,
      .health = 100.f,
      .max_health = 100.f,
      .pos = GetMousePosition(),
      .color = colors::red,
    };
    m_target_manager.spawn(new_target);
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
  DrawText(
    std::format("spaceship rotation {}", m_spaceship_movement.rotation).c_str(), 0, 0, 20,
    colors::white);

  /*auto cannon_vec = make_cannon_vectors(m_spaceship_movement);*/
  // draw circle at pos
  // get spaceship center
  DrawTextureEx(
    m_spaceship, m_spaceship_movement.position, m_spaceship_movement.rotation * 180.f/3.14f, 1.f, colors::white);
  /*const auto center = Vector2Add(m_spaceship_movement.position, {m_spaceship.width / 2.f, m_spaceship.height / 2.f});*/
  /*DrawCircleV(Vector2Add(m_spaceship_movement.position, cannon1_pos), 10.f, colors::white);*/
  DrawCircleV(m_spaceship_movement.position, 10.f, colors::red);
  DrawCircleV(Vector2Rotate(
    Vector2Add(m_spaceship_movement.position, Vector2{0,100})
    , m_spaceship_movement.rotation), 10.f, colors::white);
  // draw_both from pos
  /*DrawLineEx(*/
  /*  Vector2Rotate(Vector2Add(m_spaceship_movement.position, cannon1_pos), m_spaceship_movement.rotation), cannon_vec, 2.f, colors::white);*/
  /*/*DrawLineEx(*/
  /*/*  Vector2Rotate(Vector2Add(m_spaceship_movement.position, cannon2_pos), m_spaceship_movement.rotation), cannon_vec, 2.f, colors::white);*/
  /*const auto score_str = fmt::format("Score: {}", m_score);*/
  /*constexpr static Vector2 score_pos = {10.f, 10.f};*/
  /*DrawText(score_str.c_str(), int(score_pos.x), int(score_pos.y), 20, colors::white);*/
}
