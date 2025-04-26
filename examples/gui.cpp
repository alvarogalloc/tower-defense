#if 0
import std;
import gui.widgets;
import raylib;
import json;
int main()
{
  InitWindow(400, 400, "howdy");
  SetTargetFPS(30);
  gui::text root("Hello, World!", colors::red, 29.f);

  root.offset
    = {.x = 200.f - float(MeasureText(root.str.c_str(), int(root.font_size)) / 2), .y = 100};

  auto btn_color = colors::blue;
  btn_color.a = 0x64;
  root.add_children(
    gui::text {"This is a child text", colors::blue, 20.f},
    gui::text {"This is another child text", colors::green, 20.f},
    gui::button {
      "Click me",
      btn_color,
      50.f,
      {40.f, 30.f},
      [](gui::button& btn) {
        std::cout << "Button clicked!" << std::endl;
        btn.str = "Clicked!";
        btn.color.a = 255;
        btn.color = ColorIsEqual(btn.color, colors::blue) ? colors::red : colors::blue;
        btn.color.a = 0x64;
      },
    });
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(colors::white);
    if (WindowShouldClose()) {
      break;
    }
    // Create a text widget
    root.draw({0, 0});
    EndDrawing();
  }
  CloseWindow();
}
#endif

import gui.widgets;
import std;
import raylib;

int main()
{
    InitWindow(400, 400, "howdy");
    SetTargetFPS(30);

    using namespace gui::v2;

    container c1{
        .dir = container::direction::vertical,
        .children = {},
        .position = {0, 0},
        .size = {400, 400},
    };

    c1.add_children(
        button{"Hello, World!", colors::red, 29.f, Vector2{20.f, 20.f},
               [](auto &btn) {

               }},
        button{"Hello, World2!", colors::red, 29.f, Vector2{100.f, 10.f},
               [](auto &btn) {

               }},
        button{"Hello, World2!", colors::red, 29.f, Vector2{100.f, 10.f},
               [](auto &btn) {

               }

        }

    );

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(colors::white);
        if (WindowShouldClose())
        {
            break;
        }
        // Create a text widget
        c1.draw();
        c1.draw_box_model();
        EndDrawing();
    }
    CloseWindow();
}
