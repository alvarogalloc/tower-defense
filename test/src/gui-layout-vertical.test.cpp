import ut;
import std;
import gui;
import raylib;

int main()
{
    namespace gui = gui::v2;
    using namespace std::literals;
    using namespace boost::ut;
    using namespace rooster::colors;

    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    SetTraceLogLevel(LOG_NONE);
    InitWindow(0, 0, "testing");
    gui::container c;
    c.dir = gui::container::direction::vertical;
    for (auto color : {blue, red, green, yellow, orange, purple, black})
    {
        c.add_child(gui::label("hola"sv, color, {10,20}));
    }
    const auto s =c.get_size();
    expect(s.x == 10_i);
    expect(s.y == _i(7*20));

    CloseWindow();
}
