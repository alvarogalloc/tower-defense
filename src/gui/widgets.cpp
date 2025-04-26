module gui.widgets;
import raygui;
import raylib;

void gui::text::draw(Vector2 parent_pos)
{
  Vector2 size = MeasureTextEx(GetFontDefault(), str.c_str(), font_size, 1);
  Vector2 pos = Vector2Add(parent_pos, offset);

  DrawTextEx(GetFontDefault(), str.c_str(), pos, font_size, 1.f, color);

  for (auto i = 0; const auto& child : children) {
    child->draw(Vector2Add(pos, {0, size.y * float(++i)}));
  }
}

void gui::button::draw(Vector2 parent_pos)
{
  Vector2 size = MeasureTextEx(GetFontDefault(), str.c_str(), font_size, 1) + padding;
  Vector2 pos = Vector2Add(parent_pos, offset);
  auto previous_font_size = GuiGetStyle(DEFAULT, TEXT_SIZE);
  GuiSetStyle(0, BASE_COLOR_NORMAL, ColorToInt(color));
  GuiSetStyle(0, BASE_COLOR_FOCUSED, ColorToInt(color));
  GuiSetStyle(0, BASE_COLOR_PRESSED, ColorToInt(color));


  GuiSetStyle(DEFAULT, TEXT_SIZE, int(font_size));
  if (GuiButton({pos.x, pos.y, size.x, size.y}, str.c_str())) {
    on_click(*this);
  }
  GuiSetStyle(0, TEXT_SIZE, int(previous_font_size));
  for (auto i = 0; const auto& child : children) {
    child->draw(Vector2Add(pos, {0, size.y * float(++i)}));
  }
}

void gui::vbox::draw(Vector2)
{
  // with alignment,
}
