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
