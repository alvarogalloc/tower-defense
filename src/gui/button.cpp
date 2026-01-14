// module gui:button.impl;
module gui;
namespace gui::v2 {
void button::draw(Vector2 parent_pos) {
  if (GuiButton({parent_pos.x, parent_pos.y, get_size().x, get_size().y},
                str.c_str()))
    on_click(*this);
}

Vector2 button::get_size() const {
  return MeasureTextEx(GetFontDefault(), str.c_str(), font_size, 1.0f) +
         (padding * 2);
}

void button::set_size(Vector2 size) {
  padding = [&] {
    // Measure the natural size of the text
    const Vector2 text_size =
        MeasureTextEx(GetFontDefault(), str.c_str(), font_size, 1.0f);

    const float half = 0.5f;
    Vector2 v_new_padding = (size - text_size) * half;
    v_new_padding.x = std::max(0.0f, v_new_padding.x);
    v_new_padding.y = std::max(0.0f, v_new_padding.y);
    return v_new_padding;
  }();
}

button::button(std::string_view str, Color color, float font_size,
               Vector2 padding, std::function<void(button &)> &&on_click)
    : str(str),
      color(color),
      font_size(font_size),
      padding(padding),
      on_click(std::move(on_click)) {}

button::button(std::string_view str, Color color, Vector2 size,
               std::function<void(button &)> &&on_click)
    : str(str),
      color(color),
      font_size(0.f),
      padding(0),
      on_click(std::move(on_click)) {
  set_size(size);
}

}  // namespace gui::v2
