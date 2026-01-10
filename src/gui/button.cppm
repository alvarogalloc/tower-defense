export module gui:button;
import std;
import raylib;
import :node;
export namespace gui::v2
{

struct button : node
{
    button(std::string_view str, Color color, Vector2 size,
           std::function<void(button &)> &&on_click);
    button(std::string_view str, Color color, float font_size, Vector2 padding,
           std::function<void(button &)> &&on_click);
    void set_size(Vector2 size) override;
    [[nodiscard]] Vector2 get_size() const override;
    void draw(Vector2 parent_pos) override;

    std::string str;
    Color color;
    float font_size;
    Vector2 padding;
    std::function<void(button &)> on_click;
};
} // namespace gui::v2
