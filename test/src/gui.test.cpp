import ut;
import std;
import gui;
import raylib;

int main () {
  using namespace gui::v2;
  using namespace ut;
  SetConfigFlags(FLAG_WINDOW_HIDDEN);
  SetTraceLogLevel(LOG_NONE);
  InitWindow(0,0, "testing");
  container c;
  c.alflags = align_flags::end;
  auto btn = c.add_child(button{
  "holamundo", colors::white, 10.f,{0.2f, 10.f},
    [i =0]( button&btn) mutable {
      btn.str = "clicked!!"+ std::to_string(++i);

    }
  });
  btn->on_click(*btn);
  expect(c.children.size()  == 1_ul);
  expect(btn->str == "clicked!!1"_s);
  btn->on_click(*btn);
  btn->on_click(*btn);
  btn->on_click(*btn);
  btn->on_click(*btn);
  expect(btn->str == "clicked!!5"_s);
  CloseWindow();
}

