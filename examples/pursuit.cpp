import std;
import raylib;
import raylib_utils;

enum class action : std::uint8_t {
  add_point,
  reset_simulation,
  none,
};

action handle_input()
{
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    return action::add_point;
  }
  if (IsKeyPressed(KEY_SPACE)) {
    return action::reset_simulation;
  }
  return action::none;
}

class Pursuit {
  std::deque<Vector2> m_points;
  Vector2 current_position;
  // when the simulation starts, we'll have to calculate the bezier curve
  // current_position will be the first point in the curve and 
  // when it reaches m_points.back()+1, we pop the back and set current_position to the front

  public:
  Pursuit() = default;
  Pursuit(const std::deque<Vector2>& points)
      : m_points(points), current_position(points.front())
  {}

  void add_point(const Vector2& point)
  {
    m_points.push_back(point);
    if (m_points.size() == 1) {
      current_position = m_points.front();
    }
  }
  void reset()
  {
    m_points.clear();
  }
  void update(float delta) {
    if (m_points.size() < 2) {
      return;
    }
    constexpr static float tolerance = 5.f;
    constexpr static float speed = 250.f;
    if (Vector2Distance(current_position, m_points.at(1)) < tolerance) {
      m_points.pop_front();
      current_position = m_points.front();
    }
    // right now we'll just move the current_position towards next point
    // we'll implement the bezier curve later
    if (m_points.size() < 2) {
      return;
    }
    const auto next_position = m_points.at(1);
    const auto direction = Vector2Normalize(Vector2Subtract(next_position, current_position));
    current_position = Vector2Add(current_position, Vector2Scale(direction, speed * delta));
  }
  void draw() const
  {
    if (m_points.size() < 2) {
      return;
    }
    for (std::size_t i = 0; i < m_points.size() - 1; ++i) {
      DrawLineV(m_points.at(i), m_points.at(i + 1), colors::black);
    }

    DrawCircleV(current_position, 5.f, colors::red);
  }
};

struct game_data {
  bool simulation_started = false;
  Pursuit pursuit;
};

int main()
{
  InitWindow(400, 400, "Bezier chase");
  SetTargetFPS(60);
  /*
    we'll have three points that can be moved
    (l-shift+click for source, alt+click for control and ctrl+click for target)
    and the simulation starts when space is pressed
  */
  game_data data = {false, Pursuit()};
  while (not WindowShouldClose()) {
    float delta_time = GetFrameTime();
    /*const auto delta_time = GetFrameTime();*/
    BeginDrawing();
    ClearBackground(colors::raywhite);
    switch (handle_input()) {
      case action::add_point: {
        const auto mouse_pos = GetMousePosition();
        data.pursuit.add_point(mouse_pos);
        break;
      }
      case action::reset_simulation: data.simulation_started = true; break;
      case action::none: break;
    }
    data.pursuit.update(delta_time);
    data.pursuit.draw();
    EndDrawing();
  }
}
