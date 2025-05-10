export module gui.container;
import std;
import raylib;
import gui.node;

export namespace gui::v2
{

enum class layout_flags
{
    no_space,
    fill_space,
    space_between,
    space_around,
};
enum class align_flags
{
    center,
    start,
    end,
};

struct container
{
    enum direction
    {
        horizontal,
        vertical
    };

    auto add_child(auto &&child)
    {
        auto ret =
            std::make_shared<std::decay_t<decltype(child)>>(std::move(child));
        this->children.emplace_back(ret);
        if (ret->get_size().y > size.y)
        {
            std::cerr << "WARN: element is bigger than the container\n";
        }

        return ret;
    }

    template <typename T, typename... Args>
        requires std::derived_from<T, node>
    auto add_child(Args &&...args)
    {
        return add_child(T(std::forward<Args>(args)...));
    }

    void add_children(auto &&...appended)
    {
        (add_child(std::forward<decltype(appended)>(appended)), ...);
    }

    direction dir;
    std::vector<child_t> children;
    Vector2 position;
    Vector2 size;
    layout_flags lyflags = layout_flags::no_space;
    align_flags alflags = align_flags::start;

    void draw() const;
    void draw_box_model() const;
};

} // namespace gui::v2

