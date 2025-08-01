module;
#include "raylib.h"
#include "raymath.h"
#include <rlgl.h>
export module raylib;

export {
    using ::operator*=;
    using ::operator+=;
    using ::operator/=;
    using ::operator-=;
    using ::operator!=;
    using ::operator*;
    using ::operator+;
    using ::operator/;
    using ::operator-;
    using ::BeginShaderMode;
    using ::BeginTextureMode;
    using ::CheckCollisionCircleLine;
    using ::CheckCollisionCircleRec;
    using ::CheckCollisionCircles;
    using ::CheckCollisionLines;
    using ::CheckCollisionPointCircle;
    using ::CheckCollisionPointLine;
    using ::CheckCollisionPointPoly;
    using ::SetTextureFilter;
    using ::CheckCollisionPointRec;
    using ::CheckCollisionPointTriangle;
    using ::CheckCollisionRecs;
    using ::EndShaderMode;
    using ::EndTextureMode;
    using ::GetShaderLocation;
    using ::LoadMusicStream;
    using ::LoadShader;
    using ::Music;
    using ::PlayMusicStream;
    using ::RenderTexture;
    using ::SetShaderValue;
    using ::Shader;
    using ::StopMusicStream;
    using ::UnloadShader;
    using ::UpdateMusicStream;

    using ::CloseAudioDevice;
    using ::CloseWindow;
    using ::DrawEllipse;
    using ::DrawEllipseLines;
    using ::DrawLine;
    using ::DrawLineV;
    using ::DrawTextureEx;
    using ::GetFontDefault;
    using ::GetFrameTime;
    using ::GetMousePosition;
    // using ::GetScreenHeight;
    // using ::GetScreenWidth;
    using ::GetWindowPosition;
    using ::UnloadRenderTexture;
    using ::InitAudioDevice;
    using ::InitWindow;
    using ::rlglClose;
    using ::rlglInit;

    using ::BeginDrawing;
    using ::BeginTextureMode;
    using ::ClearBackground;
    using ::CloseWindow;
    using ::Color;
    using ::ColorToInt;
    using ::DrawLineEx;
    using ::DrawRectangleLines;
    using ::DrawRectangleLinesEx;
    using ::DrawRectanglePro;
    using ::DrawRectangleV;
    using ::DrawText;
    using ::DrawTextEx;
    using ::DrawTexturePro;
    using ::DrawTextureRec;
    using ::EndDrawing;
    using ::EndTextureMode;
    using ::Font;
    using ::GetColor;
    using ::GetRandomValue;
    using ::GetTime;
    using ::IsMouseButtonDown;
    using ::IsMouseButtonPressed;
    using ::IsMouseButtonReleased;
    using ::IsMouseButtonUp;
    using ::IsMusicStreamPlaying;
    using ::IsRenderTextureValid;
    using ::LoadFont;
    using ::LoadFontEx;
    using ::LoadMusicStream;
    using ::LoadRenderTexture;
    using ::LoadTexture;
    using ::MeasureText;
    using ::MeasureTextEx;
    using ::MouseButton;
    using ::Music;
    using ::Rectangle;
    using ::RenderTexture2D;
    using ::SetTargetFPS;
    using ::Texture2D;
    using ::UnloadFont;
    using ::UnloadMusicStream;
    using ::UnloadTexture;
    using ::Vector2;
    using ::Vector2Add;
    using ::Vector2Distance;
    using ::Vector2Equals;
    using ::Vector2Length;
    using ::Vector2Normalize;
    using ::Vector2Rotate;
    using ::Vector2Scale;
    using ::Vector2Subtract;
    using ::Vector2Zero;
    using ::WindowShouldClose;

    using ::IsMusicValid;
    using ::PlayMusicStream;
    using ::SetMasterVolume;
    using ::SetMusicPitch;
    using ::SetMusicVolume;
    using ::StopMusicStream;
    using ::UpdateAudioStream;
    using ::UpdateMusicStream;

    using ::DrawCircle;
    using ::DrawCircleGradient;
    using ::DrawCircleLines;
    using ::DrawCircleSector;
    using ::DrawCircleSectorLines;
    using ::DrawCircleV;
    using ::DrawPoly;
    using ::DrawPolyLines;
    using ::DrawRectangle;
    using ::DrawRectangleGradientEx;
    using ::DrawRectangleGradientH;
    using ::DrawRectangleGradientV;
    using ::DrawRectangleLines;
    using ::DrawRectangleLinesEx;
    using ::DrawRectangleRec;
    using ::DrawRing;
    using ::DrawText;
    using ::DrawTexture;
    using ::DrawTextureV;
    using ::DrawTriangle;
    using ::DrawTriangleFan;
    using ::DrawTriangleLines;
    using ::DrawTriangleStrip;

    using ::BeginMode2D;
    using ::Camera2D;
    using ::ColorIsEqual;
    using ::ConfigFlags;
    using ::DrawPixelV;
    using ::EndMode2D;
    using ::GetCharPressed;
    using ::GetKeyPressed;
    using ::GetRandomValue;
    using ::IsKeyDown;
    using ::IsKeyPressed;
    using ::IsKeyReleased;
    using ::IsKeyUp;
    using ::LoadSound;
    using ::PlaySound;
    using ::SetConfigFlags;
    using ::SetExitKey;
    using ::SetTraceLogCallback;
    using ::SetTraceLogLevel;
    using ::SetWindowTitle;
    using ::Sound;
    using ::TakeScreenshot;
    using ::TraceLog;
    using ::UpdateCamera;

    namespace rooster::colors
    {

    constexpr inline Color lightgray = {200, 200, 200, 255}; // Light Gray
    constexpr inline Color gray = {130, 130, 130, 255};      // Gray
    constexpr inline Color darkgray = {80, 80, 80, 255};     // Dark Gray
    constexpr inline Color yellow = {253, 249, 0, 255};      // Yellow
    constexpr inline Color gold = {255, 203, 0, 255};        // Gold
    constexpr inline Color orange = {255, 161, 0, 255};      // Orange
    constexpr inline Color pink = {255, 109, 194, 255};      // Pink
    constexpr inline Color red = {230, 41, 55, 255};         // Red
    constexpr inline Color maroon = {190, 33, 55, 255};      // Maroon
    constexpr inline Color green = {0, 228, 48, 255};        // Green
    constexpr inline Color lime = {0, 158, 47, 255};         // Lime
    constexpr inline Color darkgreen = {0, 117, 44, 255};    // Dark Green
    constexpr inline Color skyblue = {102, 191, 255, 255};   // Sky Blue
    constexpr inline Color blue = {0, 121, 241, 255};        // Blue
    constexpr inline Color darkblue = {0, 82, 172, 255};     // Dark Blue
    constexpr inline Color purple = {200, 122, 255, 255};    // Purple
    constexpr inline Color violet = {135, 60, 190, 255};     // Violet
    constexpr inline Color darkpurple = {112, 31, 126, 255}; // Dark Purple
    constexpr inline Color beige = {211, 176, 131, 255};     // Beige
    constexpr inline Color brown = {127, 106, 79, 255};      // Brown
    constexpr inline Color darkbrown = {76, 63, 47, 255};    // Dark Brown
    constexpr inline Color white = {255, 255, 255, 255};     // White
    constexpr inline Color black = {0, 0, 0, 255};           // Black
    constexpr inline Color blank = {0, 0, 0, 0};         // Blank (Transparent)
    constexpr inline Color magenta = {255, 0, 255, 255}; // Magenta
    constexpr inline Color raywhite = {245, 245, 245,
                                       255}; // My own White (raylib logo)
    } // namespace rooster::colors
}
