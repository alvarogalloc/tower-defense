module gui;

namespace gui::v2 {
void label::draw(Vector2 parent_pos) {
  auto prev=GuiGetStyle(DEFAULT, TEXT_SIZE);
  auto prev_color=GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL);
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(this->color));
  GuiSetStyle(DEFAULT, TEXT_SIZE, int(this->font_size));
  GuiLabel({parent_pos.x, parent_pos.y, get_size().x, get_size().y},
           str.c_str());
  GuiSetStyle(DEFAULT, TEXT_SIZE, prev);
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, prev_color);
}

Vector2 label::get_size() const {
  return MeasureTextEx(GetFontDefault(), str.c_str(), font_size, 1.0f) +
         (padding * 2);
}

void label::set_size(Vector2 size) {
  // Clamp to zero to avoid negative padding (optional)
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

label::label(std::string_view str, Color color, float font_size,
             Vector2 padding)
    : str(str), color(color), font_size(font_size), padding(padding) {}

label::label(std::string_view str, Color color, Vector2 size)
    : str(str), color(color), font_size(0.f), padding(0) {
  set_size(size);
}

}  // namespace gui::v2
