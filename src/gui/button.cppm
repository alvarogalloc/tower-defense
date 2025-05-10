export module gui.button;
import gui.node;
import std;
import raylib;
import raygui;
export namespace gui::v2 {

struct button : node
{
    button(std::string_view str, Color color,Vector2 size, std::function<void(button &)> &&on_click)
        : str(str), color(color), font_size(0.f), padding(0),
          on_click(std::move(on_click))
  {
    set_size(size);
  }
    button(std::string_view str, Color color, float font_size, Vector2 padding,
           std::function<void( button &)> &&on_click)
        : str(str), color(color), font_size(font_size), padding(padding),
          on_click(std::move(on_click))
    {
    }
    std::string str;
    Color color;
    float font_size;
    Vector2 padding;
    std::function<void(button &)> on_click;
    void set_size(Vector2 size) override {
         // Measure the natural size of the text
    Vector2 text_size = MeasureTextEx(GetFontDefault(), str.c_str(), font_size, 1.0f);

    // Compute new padding needed to reach target size
    Vector2 new_padding = (size - text_size) * 0.5f;

    // Clamp to zero to avoid negative padding (optional)
    new_padding.x = std::max(0.0f, new_padding.x);
    new_padding.y = std::max(0.0f, new_padding.y);

    padding = new_padding;
    }
    [[nodiscard]] Vector2 get_size() const override
    {
        return MeasureTextEx(GetFontDefault(), str.c_str(), font_size, 1.0f) +
               (padding * 2);
    }
    void draw(Vector2 parent_pos)  override
    {
        if (GuiButton({parent_pos.x, parent_pos.y, get_size().x, get_size().y},
                  str.c_str())) on_click(*this);
    }
};
}
