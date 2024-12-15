module bullets;
import std;
import debug;
import fmt;
import raygui;

// implement constructor, add_point, reset and update
bullet_chase::bullet_chase(const std::vector<Vector2>& points, float speed)
    : m_points(points)
    , m_speed(speed)
{
  if (m_points.size() > 0) {
    current_position = m_points.front();
  }
}

void bullet_chase::add_point(const Vector2& point)
{
  m_points.push_back(point);
  if (m_points.size() == 1) {
    current_position = m_points.front();
  }
}

void bullet_chase::reset()
{
  m_points.clear();
  current_position = {0, 0};
}

void bullet_chase::update(float delta)
{
  if (m_points.size() < 2) {
    return;
  }
  constexpr static float tolerance = 3.f;
  if (Vector2Distance(current_position, m_points.at(1)) < tolerance) {
    m_points.erase(m_points.begin());
    current_position = m_points.front();
  }
  // right now we'll just move the current_position towards next point
  // we'll implement the bezier curve later
  if (m_points.size() < 2) {
    return;
  }
  const auto next_position = m_points.at(1);
  const auto direction = Vector2Normalize(Vector2Subtract(next_position, current_position));
  current_position = Vector2Add(current_position, Vector2Scale(direction, m_speed * delta));
}

// the bullets will be circling in three ellipses around the player
// in angles 0, 120, 240 (in radians: 0, 2pi/3, 4pi/3)

constexpr static auto pi = std::numbers::pi_v<float>;

bullets::info bullets::info::load(std::string_view filename)
{
  std::ifstream file {filename.data()};
  debug::my_assert(file.is_open(), "Could not load file");
  bullets::info bullet_info;

  std::string line;
  while (std::getline(file, line)) {
    // Split line into name and value
    std::istringstream iss(line);
    std::string name;
    iss >> name;

    if (name == "damage") {
      float value;
      iss >> value;
      bullet_info.damage = value;
    } else if (name == "radius") {
      float value;
      iss >> value;
      bullet_info.radius = value;
    } else if (name == "speed") {
      float value;
      iss >> value;
      bullet_info.speed = value;
    } else if (name == "count") {
      std::size_t value;
      iss >> value;
      bullet_info.count = value;
    } else if (name == "color") {
      unsigned int hex_color;
      iss >> std::hex >> hex_color; // read hex value
      bullet_info.color = GetColor(hex_color);
    } else if (name == "ellipse_radius") {
      float x, y;
      iss >> x >> y;
      bullet_info.ellipse_radius = Vector2 {x, y};
    }
  }

  return bullet_info;
}

bullets::bullets(const bullets::info& bullets_info)
    : m_info {bullets_info}
{
  m_bullets.resize(m_info.count);
  std::ranges::fill(m_bullets, bullet {.alive = true, .acc_time = 0.f});
  float init = 0.f;
  const float step = 2 * pi / static_cast<float>(m_info.count);
  for (auto& b : m_bullets) {
    b.acc_time = init;
    init += step;
  }
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
    DrawCircleV(pos, m_info.radius, m_info.color);
  }
}
void bullets::draw(const Vector2 center, bool draw_ellipse) const
{
  draw(center);
  if (draw_ellipse) {
    DrawEllipseLines(
      int(center.x), int(center.y), m_info.ellipse_radius.x, m_info.ellipse_radius.y, m_info.color);
  }
}

void bullets::respawn_dead()
{
  for (auto& b : m_bullets | std::views::filter([](const auto& b) {
                   return !b.alive;
                 })) {
    // the position varies with acc_time
    // i want to spawn the bullets in separate position
    // random values are not good because some bullets will be spawned on top of each other
    // so i will use the same position for all bullets
    b.alive = true;
  }
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
      std::cos(res->acc_time) * m_info.speed, std::sin(res->acc_time) * m_info.speed};

    detached.position = pos;
    detached.velocity = vel;
    detached.info = &m_info;
    res->alive = false;
  }

  detached.chase.reset();
  detached.chase.add_point(detached.position);
  // calculate a second point that is bulletsx and targety
  detached.chase.add_point(Vector2 {detached.position.x, detached.target_position.y});
  detached.chase.add_point(detached.target_position);

  return detached;
}
Vector2 bullets::get_edge_for(const Vector2 target, const Vector2 center) const
{
  Vector2 direction = Vector2Subtract(target, center);
  float angle = std::atan2(direction.y, direction.x);
  return Vector2 {
    std::cos(angle) * m_info.ellipse_radius.x + center.x,
    std::sin(angle) * m_info.ellipse_radius.y + center.y,
  };
}

void bullets::update(const float delta)
{
  // only one ellipse now
  for (auto& b : m_bullets) {
    b.acc_time += delta;
    if (b.acc_time > 2 * pi) {
      b.acc_time = 0.f;
    }
  }
}
