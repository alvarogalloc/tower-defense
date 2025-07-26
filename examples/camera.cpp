import std;
import raylib;

int main()
{
    using namespace rooster;
    InitWindow(400, 400, "Camera Example!!");

    Camera2D camera;
    Rectangle player_rect{200, 200, 50, 50};

    camera.target = {.x = 20.0f + player_rect.x, .y = 20.0f + player_rect.y};
    camera.offset = {.x = 400.f / 2.0f, .y = 400.f / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    const float player_vel{500.f};
    SetTargetFPS(30);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        camera.target = {.x=player_rect.x, .y=player_rect.y};
        Vector2 newp = Vector2Scale({.x = player_rect.x, .y = player_rect.y},
                                    player_vel * GetFrameTime());
        player_rect.x = newp.x;
        player_rect.y = newp.y;
        BeginMode2D(camera);

        ClearBackground(colors::white);

        DrawRectangleRec(player_rect, colors::black);
        BeginMode2D(camera);
        EndDrawing();
    }
    CloseWindow();
}
