export module gui.widgets;
import raylib;
import raygui;
import std;

export namespace gui::v2
{
struct node
{
    virtual ~node() = default;
    [[nodiscard]] virtual Vector2 get_size() const = 0;
    virtual void draw(Vector2 parent_pos) const = 0;
};

using child_t = std::shared_ptr<node>;
struct button : node
{
    button(std::string_view str, Color color, float font_size, Vector2 padding,
           std::function<void(button &)> &&on_click)
        : str(str), color(color), font_size(font_size), padding(padding),
          on_click(std::move(on_click))
    {
    }
    std::string str;
    Color color;
    float font_size;
    Vector2 padding;
    std::function<void(button &)> on_click;
    [[nodiscard]] Vector2 get_size() const override
    {
        return MeasureTextEx(GetFontDefault(), str.c_str(), font_size, 1.0f) +
               (padding * 2);
    }
    void draw(Vector2 parent_pos) const override
    {
        GuiButton({parent_pos.x, parent_pos.y, get_size().x, get_size().y},
                  str.c_str());
    }
};

struct container
{
    enum direction
    {
        horizontal,
        vertical
    };

    template <typename T, typename... Args>
        requires std::derived_from<T, node>
    auto &add_child(Args &&...args)
    {
        return this->children.emplace_back(
            std::make_shared<T>(std::forward<Args>(args)...));
    }

    void add_children(auto &&...appended)
    {
        (add_child(std::forward<decltype(appended)>(appended)), ...);
    }

    auto &add_child(auto &&child)
    {
        return this->children.emplace_back(
            std::make_shared<std::decay_t<decltype(child)>>(child));
    }
    direction dir;
    std::vector<child_t> children;
    Vector2 position;
    Vector2 size;

    void draw() const;
    void draw_box_model() const;
};

#define __feat_layouts 0
#if __feat_layouts
enum auto_layout_flags
{
    start_on_end,
    fill_space,
    space_between,
    space_around,
};
enum align_flags
{
    align_center,
    align_start,
    align_end,
};
#endif
} // namespace gui::v2

module :private;

namespace gui::v2
{
void container::draw() const
{
    Vector2 pos = this->position;
    for (const auto &child : this->children)
    {
        // log warning when out of bounds
        float checked_dimension = dir == direction::horizontal ? pos.x : pos.y;
        float dimension_limit =
            dir == direction::horizontal ? this->size.x : this->size.y;
        if (checked_dimension > dimension_limit)
        {
            std::cerr << "Child widget exceeds container bounds!" << std::endl;
            return;
        }

        child->draw(pos);
        if (this->dir == direction::horizontal)
        {
            pos.x += child->get_size().x;
        }
        else
        {
            pos.y += child->get_size().y;
        }
    }
}

void container::draw_box_model() const
{
    // draw a rectangle from pos to pos+ sizeof each child

    DrawRectangleLinesEx(
        {this->position.x, this->position.y, this->size.x, this->size.y}, 1,
        colors::red);
    Vector2 pos = this->position;
    std::accumulate(
        std::begin(children), std::end(children), pos,
        [this](Vector2 acc, const child_t &child) {
            if (this->dir == direction::horizontal)
            {
                DrawRectangleLinesEx(
                    {acc.x, acc.y, child->get_size().x, child->get_size().y}, 1,
                    colors::blue);
                return Vector2{acc.x + child->get_size().x, acc.y};
            }
            else
            {
                DrawRectangleLinesEx(
                    {acc.x, acc.y, child->get_size().x, child->get_size().y}, 1,
                    colors::blue);
                return Vector2{acc.x, acc.y + child->get_size().y};
            }
        });
}

} // namespace gui::v2
