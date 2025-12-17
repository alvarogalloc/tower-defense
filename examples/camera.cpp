import std;
import raylib;

auto get_movement() {
  Vector2 dir{0, 0};
  if (IsKeyDown(KEY_W)) {
    dir.y -= 1;
  }
  if (IsKeyDown(KEY_A)) {
    dir.x -= 1;
  }
  if (IsKeyDown(KEY_S)) {
    dir.y += 1;
  }
  if (IsKeyDown(KEY_D)) {
    dir.x += 1;
  }
  return Vector2Normalize(dir);
}

int main() {
  using namespace rooster;
  const int w_size{400};
  InitWindow(w_size, w_size, "Camera Example!!");

  Rectangle player_rect{200, 200, 50, 50};

  const auto make_camera = [&] {
    Camera2D camera;
    camera.target = {.x = player_rect.x, .y = player_rect.y};
    camera.offset = {.x = 400.f / 2.0f, .y = 400.f / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    return camera;
  };
  Camera2D camera{make_camera()};
  const float player_vel{200.f};
  const int fps{60};
  SetTargetFPS(fps);
  while (!WindowShouldClose()) {
    BeginDrawing();
    Vector2 newp = Vector2Scale(get_movement(), player_vel * GetFrameTime());
    player_rect.x += newp.x;
    player_rect.y += newp.y;

    if (IsKeyPressed(KEY_SPACE)) {
      // center the camera
      camera.target = {.x = player_rect.x, .y = player_rect.y};
      std::println("camera pos {} {}", camera.target.x, camera.target.y);
    }
    BeginMode2D(camera);

    ClearBackground(colors::white);

    DrawRectangleRec(player_rect, colors::black);
    BeginMode2D(camera);
    EndDrawing();
  }
  CloseWindow();
}
