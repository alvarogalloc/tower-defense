import ut;
import std;
import gui;
import raylib;

int main()
{
    namespace gui = gui::v2;
    using namespace std::literals;
    using namespace boost::ut;

    SetConfigFlags(FLAG_WINDOW_HIDDEN);
    SetTraceLogLevel(LOG_NONE);
    InitWindow(0, 0, "testing");
    gui::container c;
    c.alflags = gui::h_alignment::left;
    auto btn =
        c.add_child(gui::button{"holamundo",
                                rooster::colors::white,
                                10.f,
                                {0.2f, 10.f},
                                [i = 0](gui::button &btn) mutable {
                                    btn.str = "clicked!!" + std::to_string(++i);
                                }});
    btn->on_click(*btn);
    expect(c.children.size() == 1_ul);
    expect(that % btn->str == "clicked!!1"s);
    btn->on_click(*btn);
    btn->on_click(*btn);
    btn->on_click(*btn);
    btn->on_click(*btn);
    expect(that % btn->str == "clicked!!5"s);
    CloseWindow();
}
