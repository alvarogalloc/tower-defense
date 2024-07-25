module game_scene;
import raygui;
import fmt;
import debug;
namespace {
const float frame_size = 32.f;

Rectangle house_rect()
{
  return { static_cast<float>(GetScreenWidth()) / 2 - 64,
    static_cast<float>(GetScreenHeight()) / 2 - 64,
    128,
    128 };
}
constexpr bool doOverlap(const Rectangle &rect1, const Rectangle &rect2)
{
  if (rect1.x + rect1.width <= rect2.x || rect2.x + rect2.width <= rect1.x)
  {
    return false;
  }
  if (rect1.y + rect1.height <= rect2.y || rect2.y + rect2.height <= rect1.y)
  {
    return false;
  }
  return true;
}


template<std::size_t size>
void respawn_dead(std::array<enemy, size> &enemies, std::size_t count)
{
  enum class spawn_point { top, left, right, bottom };
  spawn_point current_spawn{ spawn_point::top };
  fmt::print(info, "Respawning dead enemies\n");
  for (auto &enemy : enemies | std::views::filter([](const auto &el) {
         return el.health == 0;
       }) | std::views::take(count))
  {
    using enum spawn_point;
    enemy.health = 100;
    Vector2 spawn{ 0, 0 };
    auto rand_x = []() { return float(GetRandomValue(0, GetScreenWidth())); };
    auto rand_y = []() { return float(GetRandomValue(0, GetScreenHeight())); };
    if (current_spawn == top || current_spawn == bottom)
    {
      if (current_spawn == bottom) { spawn.y = float(GetScreenHeight()); }
      spawn.x = rand_x();
    } else
    {
      if (current_spawn == right) { spawn.x = float(GetScreenWidth()); }
      spawn.y = rand_y();
    }
    enemy.position = spawn;
    switch (current_spawn)
    {
    case top:
      current_spawn = left;
      break;
    case left:
      current_spawn = right;
      break;
    case right:
      current_spawn = bottom;
      break;
    case bottom:
      current_spawn = top;
      break;
    }
  }
}

template<std::size_t size>
void respawn_bullets(std::array<bullet, size> &bullets, std::size_t count)
{

  for (auto &b : bullets | std::views::filter([](const auto &el) {
         return el.hit_count != el.max_hit_count;
       }) | std::views::take(count))
  {
    b.hit_count = 0;
  }
}

const float bullet_speed = 300.f;
const float bullet_radius = 5.f;
}// namespace

void game_scene::on_start()
{

  m_music = LoadMusicStream(SRC_DIR "/assets/title-theme.ogg");
  m_music.looping = true;
  PlayMusicStream(m_music);
  m_enemy_frames = LoadTexture(SRC_DIR "/assets/blob_enemy-Sheet.png");
  m_grass = LoadTexture(SRC_DIR "/assets/grass.png");
  enemy proto_enemy{ .position = { 0, 0 },
    .frame = 0,
    .frame_counter = 0,
    .frame_speed = 15,
    .health = 0,
    .speed = 50.f };

  enemies.fill(proto_enemy);
  respawn_dead(enemies, enemies.size());


  // the all spawn from the house
  // the direction is to the mouse (todo in update)
  // hit_count = max_hit_count means bullet is dead so no damage
  bullet proto_bullet{ .hit_count = 0,
    .max_hit_count = 5,
    .damage = 100,
    .position = { house_rect().x, house_rect().y },
    .direction = { 0, 0 } };
  bullets.fill(proto_bullet);
  for (auto &b : bullets)
  {
    b.position = { static_cast<float>(GetRandomValue(int(house_rect().x),
                     int(house_rect().x + house_rect().width))),
      static_cast<float>(GetRandomValue(
        int(house_rect().y), int(house_rect().y + house_rect().height))) };
  }
}

void game_scene::on_update()
{
  UpdateMusicStream(m_music);
  // center
  auto [center_x, center_y] = Vector2{ static_cast<float>(GetScreenWidth()) / 2,
    static_cast<float>(GetScreenHeight()) / 2 };
  const float acceleration = 5.0f;
  const float max_speed = 300.0f;
  for (auto &enemy : enemies)
  {
    enemy.speed += acceleration * GetFrameTime();
    if (enemy.speed > max_speed) enemy.speed = max_speed;
    // move towards the mouse
    auto dx = center_x - enemy.position.x;
    auto dy = center_y - enemy.position.y;
    auto angle = std::atan2(dy, dx);
    enemy.position.x += std::cos(angle) * GetFrameTime() * enemy.speed;
    enemy.position.y += std::sin(angle) * GetFrameTime() * enemy.speed;

    if (doOverlap(
          house_rect(), { enemy.position.x, enemy.position.y, 32.f, 32.f }))

    {
      enemy.health = 0;
    }
    enemy.frame_counter++;
    if (enemy.frame_counter >= (60 / enemy.frame_speed))
    {
      enemy.frame_counter = 0;
      enemy.frame++;
      if (enemy.frame > 3) enemy.frame = 0;
    }
  }
  // if there are less than 5 enemies, respawn 5
  if (std::ranges::count_if(
        enemies, [](const auto &e) { return e.health != 0; })
      < 5)
  {
    respawn_dead(enemies, 5);
  }

  // move bullets
  auto mouse = GetMousePosition();
  auto distance = [](Vector2 a, Vector2 b) {
    return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
  };
  auto normalize = [](Vector2 v) {
    auto length = std::sqrt(v.x * v.x + v.y * v.y);
    return Vector2{ v.x / length, v.y / length };
  };
  for (auto &b : bullets | std::views::filter([](const auto &el) {
         return el.hit_count != el.max_hit_count;
       }))
  {
    const auto threshold = 50;
    const int rand = GetRandomValue(-threshold, threshold);
    const auto dx = mouse.x - b.position.x;
    const auto dy = mouse.y - b.position.y;
    const auto dist = distance(mouse, b.position);
    if ( dist > float(threshold))
    {
      b.direction = normalize({ dx + rand, dy + rand });
      b.position.x += b.direction.x * GetFrameTime() * bullet_speed;
      b.position.y += b.direction.y * GetFrameTime() * bullet_speed;
    } else
    {
      // idle bullet
      const float near_speed = 100.f;
      // also rotate the bullet
      const auto angle = std::atan2(b.direction.y, b.direction.x);
      b.direction = normalize(
        { std::cos(angle + GetFrameTime()), std::sin(angle + GetFrameTime()) });

      b.position.x -= b.direction.x * GetFrameTime() * near_speed;
      b.position.y -= b.direction.y * GetFrameTime() * near_speed;
    }
  }
}

std::unique_ptr<scene> game_scene::on_exit()
{
  StopMusicStream(m_music);

  UnloadMusicStream(m_music);
  UnloadTexture(m_enemy_frames);
  UnloadTexture(m_grass);

  return nullptr;
}

void game_scene::on_render()
{
  ClearBackground(colors::white);
  const int n_tiles_w = GetScreenWidth() / 32;
  const int n_tiles_h = (GetScreenHeight() - 64) / 32;
  for (int i : std::views::iota(0, n_tiles_w))
  {
    for (int j : std::views::iota(0, n_tiles_h))
    {
      DrawTextureRec(m_grass,
        { 0, 0, 32, 32 },
        { float(i) * 32, float(j) * 32 },
        colors::white);
    }
  }

  for (auto &enemy :
    enemies | std::views::filter([](const auto &e) { return e.health == 0; }))
  {
    const Rectangle enemy_rect{
      enemy.position.x, enemy.position.y, frame_size, frame_size
    };
    DrawTexturePro(m_enemy_frames,
      Rectangle{ static_cast<float>(frame_size * enemy.frame),
        0,
        frame_size,
        frame_size },
      enemy_rect,
      { 0, 0 },
      0,
      colors::white);
  }

  DrawRectangle(20, 20, GetScreenWidth() - 40, 60, colors::yellow);
  DrawText("Money: $ ", 20, 20, 45, colors::black);
  DrawText("Life: 100 ", 400, 20, 45, colors::black);
  DrawRectangleLinesEx(house_rect(), 3.f, colors::black);

  static int list_scroll_index = 0;
  static int list_active = -1;
  std::string list_items = "";
  for (int i = 0;
       const auto &bullet : bullets | std::views::filter([](const auto &el) {
         return el.hit_count != el.max_hit_count;
       }))
  {
    list_items += fmt::format("Bullet #{} (x: {:.2f} y: {:.2f});",
      i,
      bullet.position.x,
      bullet.position.y);
    ++i;
    DrawCircle(int(bullet.position.x),
      int(bullet.position.y),
      bullet_radius,
      colors::red);
  }
  if (list_items.size() > 0)
  {
    list_items.pop_back();
    GuiListView(Rectangle{ 165, 25, 250, 124 },
      list_items.c_str(),
      &list_scroll_index,
      &list_active);
  }

  // DrawRectanglePro(house_rect(), { 0, 0 }, 0, colors::red);
}
