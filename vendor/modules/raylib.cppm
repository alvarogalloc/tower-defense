module;
#include "raylib.h"
export module raylib;


export
{
  using ::InitWindow;
  using ::InitAudioDevice;
  using ::CloseAudioDevice;
  using ::CloseWindow;
  using ::GetWindowPosition;
  using ::GetScreenWidth;
  using ::GetScreenHeight;
  using ::DrawTextureEx;
using ::GetFrameTime;
using ::GetMousePosition;

  using ::SetTargetFPS;
  using ::WindowShouldClose;
  using ::BeginDrawing;
  using ::ClearBackground;
  using ::GetColor;
  using ::EndDrawing;
  using ::Color;
  using ::CloseWindow;
  using ::Rectangle;
  using ::DrawRectanglePro;
  using ::DrawRectangleV;
using ::DrawTexturePro;
  using ::LoadFont;
  using ::LoadTexture;
  using ::LoadMusicStream;
  using ::GetRandomValue;
  using ::UnloadMusicStream;
  using ::UnloadTexture;
  using ::UnloadFont;
  using ::Texture2D;
  using ::RenderTexture2D;
using ::LoadRenderTexture;
  using ::Music;
using ::LoadFontEx;
using ::DrawTextEx;
using ::BeginTextureMode;
using ::EndTextureMode;
using ::DrawTextureRec;
using ::IsRenderTextureReady;
  using ::MeasureText;
  using ::MeasureTextEx;
  using ::IsMusicStreamPlaying;
  using ::Font;
  using ::Vector2;
using ::DrawRectangleLinesEx;
using ::DrawRectangleLines;

  using ::UpdateAudioStream;
  using ::UpdateMusicStream;
  using ::PlayMusicStream;
  using ::StopMusicStream;
  using ::SetMusicVolume;
  using ::SetMusicPitch;
  using ::IsMusicReady;
  using ::SetMasterVolume;

  using ::DrawText;
  using ::DrawTexture;
  using ::DrawRectangle;
  using ::DrawRectangleRec;
  using ::DrawRectangleLines;
  using ::DrawRectangleLinesEx;
  using ::DrawRectangleGradientV;
  using ::DrawRectangleGradientH;
  using ::DrawRectangleGradientEx;
  using ::DrawRectangleLines;
  using ::DrawRectangleLinesEx;
  using ::DrawTriangle;
  using ::DrawTriangleLines;
  using ::DrawTriangleFan;
  using ::DrawTriangleStrip;
  using ::DrawPoly;
  using ::DrawPolyLines;
  using ::DrawCircle;
  using ::DrawCircleSector;
  using ::DrawCircleSectorLines;
  using ::DrawCircleGradient;
  using ::DrawCircleV;
  using ::DrawCircleLines;
  using ::DrawRing;


  using ::IsKeyPressed;
  using ::IsKeyDown;
  using ::IsKeyReleased;
  using ::IsKeyUp;
  using ::GetKeyPressed;
  using ::GetCharPressed;
  using ::SetExitKey;
  using ::SetTraceLogLevel;
  using ::SetConfigFlags;
  using ::SetTraceLogCallback;
  using ::TraceLog;
  using ::TakeScreenshot;
  using ::GetRandomValue;

  namespace colors {

    constexpr Color lightgray = { 200, 200, 200, 255 };// Light Gray
    constexpr Color gray = { 130, 130, 130, 255 };// Gray
    constexpr Color darkgray = { 80, 80, 80, 255 };// Dark Gray
    constexpr Color yellow = { 253, 249, 0, 255 };// Yellow
    constexpr Color gold = { 255, 203, 0, 255 };// Gold
    constexpr Color orange = { 255, 161, 0, 255 };// Orange
    constexpr Color pink = { 255, 109, 194, 255 };// Pink
    constexpr Color red = { 230, 41, 55, 255 };// Red
    constexpr Color maroon = { 190, 33, 55, 255 };// Maroon
    constexpr Color green = { 0, 228, 48, 255 };// Green
    constexpr Color lime = { 0, 158, 47, 255 };// Lime
    constexpr Color darkgreen = { 0, 117, 44, 255 };// Dark Green
    constexpr Color skyblue = { 102, 191, 255, 255 };// Sky Blue
    constexpr Color blue = { 0, 121, 241, 255 };// Blue
    constexpr Color darkblue = { 0, 82, 172, 255 };// Dark Blue
    constexpr Color purple = { 200, 122, 255, 255 };// Purple
    constexpr Color violet = { 135, 60, 190, 255 };// Violet
    constexpr Color darkpurple = { 112, 31, 126, 255 };// Dark Purple
    constexpr Color beige = { 211, 176, 131, 255 };// Beige
    constexpr Color brown = { 127, 106, 79, 255 };// Brown
    constexpr Color darkbrown = { 76, 63, 47, 255 };// Dark Brown

    constexpr Color white = { 255, 255, 255, 255 };// White
    constexpr Color black = { 0, 0, 0, 255 };// Black
    constexpr Color blank = { 0, 0, 0, 0 };// Blank (Transparent)
    constexpr Color magenta = { 255, 0, 255, 255 };// Magenta
    constexpr Color raywhite = { 245,
      245,
      245,
      255 };// My own White (raylib logo)
  }// namespace colors
}
