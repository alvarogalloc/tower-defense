import gui.widgets;
import std;
import raylib;
import raygui;

int main()
{
    InitWindow(400, 400, "howdy");
    SetTargetFPS(30);

    using namespace gui::v2;

    // container c1{
    //     .dir = container::direction::vertical,
    //     .children = {},
    //     .position = {0, 0},
    //     .size = {400, 400},
    // };

    container bottom_menu{
        .dir = container::direction::horizontal,
        .children = {},
        .position = {0, 400 - 50},
        .size = {400, 50},
    };
    bottom_menu.add_child(
        button{GuiIconText(ICON_WINDOW, ""), colors::green, 20.f, Vector2{0, 8},
               [](auto &btn) { std::cout << "Hello, World!" << std::endl; }});

        bottom_menu.add_child(
        button{GuiIconText(ICON_CROSS, ""), colors::red, 20.f, Vector2{0, 8},
               [](auto &btn) { std::cout << "Hello, World!" << std::endl; }});


    // c1.add_children(
    //     button{"Hello, World!", colors::red, 29.f, Vector2{20.f, 20.f},
    //            [](auto &btn) {
    //
    //            }},
    //     button{"Hello, World2!", colors::red, 29.f, Vector2{100.f, 10.f},
    //            [](auto &btn) {
    //
    //            }},
    //     button{"Hello, World2!", colors::red, 29.f, Vector2{100.f, 10.f},
    //            [](auto &btn) {
    //
    //            }
    //
    //     }
    //
    // );

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(colors::white);
        if (WindowShouldClose())
        {
            break;
        }
        // Create a text widget
        // c1.draw();
        // c1.draw_box_model();
        bottom_menu.draw();
        EndDrawing();
    }
    CloseWindow();
}
