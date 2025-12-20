#include <algorithm>
#include <cmath>
import raylib;
import std;

struct add_point_data
{
    Vector2 new_point;
};

struct move_point_data
{
    Vector2 new_point;
};

enum class app_action
{
    exit,
    reset,
    none
};
using action_data = std::variant<add_point_data, move_point_data, app_action>;

[[nodiscard]] auto get_selected_point(std::span<Vector2> polygon_points) -> std::optional<std::size_t>
{
    if (polygon_points.empty())
        return {};
    std::size_t selected_i{0};
    auto min_distance = std::numeric_limits<float>::max();
    const auto [x1, y1] = GetMousePosition();
    for (std::size_t i{0}; i < polygon_points.size(); i++)
    {
        const auto &[x2, y2] = polygon_points[i];
        if (const float dis = std::hypot(x2 - x1, y2 - y1); dis > min_distance)
        {
            selected_i = i;
            min_distance = dis;
        }
    }
    return selected_i;
}
auto handle_input() -> action_data
{
    using enum app_action;
    if (IsKeyPressed(KEY_R))
    {
        return reset;
    }
    else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        return add_point_data{GetMousePosition()};
    }
    return none;
}
template <typename... ts> struct visitor : ts...
{
    using ts::operator()...;
};

int main()
{
    const int w = 400, h = 400;
    const int fps = 60;
    // will compute at most this points per spline
    const int max_points_per_spline = 1 << 8;
    const int n_sides_control_polygon{4};

    InitWindow(w, h, "Bezier chase");
    SetTargetFPS(fps);

    /*
      we'll have four points that can be moved
      (l-shift+click for source, alt+click for control and ctrl+click for
      target) and the simulation starts when space is pressed
    */
    std::vector<Vector2> convex_polygon{};
    std::vector<Vector2> points{};
    points.reserve(max_points_per_spline);
    float acc{0.f};

    auto reset_animation = [&convex_polygon, &acc]() {
        convex_polygon.clear();
        acc = 0.f;
    };
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        acc += dt;

        BeginDrawing();
        ClearBackground(rooster::colors::raywhite);

        std::visit(visitor{[&reset_animation](const app_action c) {
                               using enum app_action;
                               switch (c)
                               {
                               case none:
                                   return;
                               case exit:
                                   CloseWindow();
                                   break;
                               case reset:
                                   reset_animation();
                                   break;
                               }
                           },

                           [&convex_polygon](const add_point_data p) {
                               if (convex_polygon.size() < n_sides_control_polygon)
                               {
                                   convex_polygon.push_back(p.new_point);
                               }
                           },
                           [&convex_polygon](const move_point_data p) {
                               if (auto selected = get_selected_point(convex_polygon))
                               {
                                   convex_polygon[selected.value()] = p.new_point;
                               }
                           }},
                   handle_input());

        EndDrawing();
    }
    CloseWindow();
}
