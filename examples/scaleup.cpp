import std;
import raylib;

int main()
{
    using namespace rooster;
    const float resx = 320;
    const float resy = 180;
    const float scale = 5.f;
    const float wx = resx*scale;
    const float wy = resy*scale;
    const int fps = 30;
    InitWindow(int(wx), int(wy), "Scale texture Example!!");
    SetTargetFPS(fps);
    RenderTexture target = LoadRenderTexture(int(resx), int(resy));
    // SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    while (!WindowShouldClose())
    {
        BeginTextureMode(target);
        ClearBackground(colors::white);
        const Vector2 size{resx / 2, resy / 2};
        DrawRectangleV({0, 0}, size, rooster::colors::gray);
        DrawRectangleV({resx / 2, 0}, size, rooster::colors::orange);
        DrawRectangleV({0, resy / 2}, size, rooster::colors::red);
        DrawRectangleV({resx / 2, resy / 2}, size, rooster::colors::green);

        EndTextureMode();

        BeginDrawing();
        DrawTexturePro(target.texture, {0, -resy, resx, -resy},
                       {0, 0, wx, wy}, {0, 0}, 0.f,
                       rooster::colors::white);

        EndDrawing();
    }
    UnloadRenderTexture(target);
    CloseWindow();
}
