import std;
import raylib;
import raygui;
import gui;
using namespace gui::v2;

container make_bottom_bar()
{

    container bottom_menu{
        .dir = container::direction::horizontal,
        .children = {},
        .position = {0, 200},
        .size = {200, 50},
        .lyflags = layout_flags::no_space,
        .alflags = align_flags::start,

    };
    // maybe a pipe to fit_to(xyvec) that recomputes the fontsize and padding to
    // fit gui btn = btn{...} | fit_to (box_size) | align_to(left(container))
    auto btn = bottom_menu.add_child<button>(
        "First", colors::green, Vector2{50, 50},
        [](auto &) { std::cout << "Hello, World!" << std::endl; });

    auto btn2 = bottom_menu.add_child<button>(
        "second", colors::green, Vector2{50, 50},
        [](auto &) { std::cout << "Hello, World!" << std::endl; });
    auto btn3 = bottom_menu.add_child<button>(
        "third", colors::green, Vector2{50, 50},
        [](auto &) { std::cout << "Hello, World!" << std::endl; });

    auto btn4 = bottom_menu.add_child<button>(
        "hola", colors::green, Vector2{50, 50},
        [](auto &) { std::cout << "Hello, World!" << std::endl; });

    // bottom_menu.add_child(
    //
    //     button{"Second", colors::red, 20.f, Vector2{0, 8},
    //            [](auto &) { std::cout << "Hello, World!" << std::endl; }});
    // bottom_menu.position.y -= btn->get_size().y;
    return bottom_menu;
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(400, 600, "howdy");
    SetTargetFPS(30);

    using gui_t = std::pair<std::string_view, container>;
    gui_t bottom_bar{"bottom_bar", make_bottom_bar()};
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
        bottom_bar.second.draw();
        bottom_bar.second.draw_box_model();
        EndDrawing();
    }
    CloseWindow();
}
