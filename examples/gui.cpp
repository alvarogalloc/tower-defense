import std;
import raylib;
import gui;

using namespace gui::v2;
using namespace rooster;

container make_bottom_bar()
{
    container bottom_menu{.size = {300, 100},
                          .position = {50, 200},
                          .gap = 1.f,
                          .dir = container::direction::vertical,
                          .children = {},
                          .lyflags = layout_flags::no_space,
                          .alflags = h_alignment::center};
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
        "fourth", colors::green, Vector2{100, 50},
        [](auto &) { std::cout << "Hello, World!" << std::endl; });
    auto btn5 = bottom_menu.add_child<button>(
        "fifth", colors::green, Vector2{30, 50},
        [](auto &) { std::cout << "Hello, World!" << std::endl; });
    auto subcont =
        bottom_menu.add_child(container{.size = {100, 100},
                                        .position = {0, 0},
                                        .gap = 0.f,
                                        .dir = container::direction::vertical,
                                        .children = {},
                                        .lyflags = layout_flags::no_space,
                                        .alflags = h_alignment::center});
    subcont->add_child<button>(
        "mini", colors::green, Vector2{30, 50},
        [](auto &) { std::cout << "Hello, World!" << std::endl; });
    return bottom_menu;
}

int main()
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    SetTraceLogLevel(LOG_WARNING | LOG_ERROR);
    const int win_x{400}, win_y{600};
    InitWindow(win_x, win_y, "howdy");
    const int target_fps{30};
    SetTargetFPS(target_fps);

    using gui_t = std::pair<std::string_view, container>;
    gui_t bottom_bar{"bottom_bar", make_bottom_bar()};
    bool allow_move_container_to_mouse = false;
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(colors::white);
        if (IsKeyPressed(KEY_Q))
        {
            break;
        }
        if (IsKeyPressed(KEY_M))
        {
            allow_move_container_to_mouse = !allow_move_container_to_mouse;
        }

        if (allow_move_container_to_mouse)
        {
            bottom_bar.second.position = GetMousePosition();
        }
        bottom_bar.second.draw();
        auto txt = std::string("horizontal alignment:") +
                   to_string(bottom_bar.second.alflags).data();
        const Vector2 debugpos{100, 50};
        const auto font_size{20.f};
        // DrawText(txt.c_str(), debugpos.x, debugpos.y, font_size,
        // colors::brown);
        DrawTextEx(GetFontDefault(), txt.c_str(), debugpos, font_size, 1.f,
                   colors::brown);

        if (IsKeyPressed(KEY_L))
        {

            using enum gui::v2::container::direction;
            bottom_bar.second.dir =
                bottom_bar.second.dir == horizontal ? vertical : horizontal;
        }
        if (IsKeyPressed(KEY_C))
        {
            using enum gui::v2::h_alignment;
            switch (bottom_bar.second.alflags)
            {
            case left:
                bottom_bar.second.alflags = right;
                break;
            case right:
                bottom_bar.second.alflags = center;
                break;
            case center:
                bottom_bar.second.alflags = left;
                break;
            }
        }

        bottom_bar.second.draw_box_model();
        EndDrawing();
    }
    CloseWindow();
}
