export module gui.node;
import std;
import raylib;

export namespace gui::v2
{
struct node
{
    virtual ~node() = default;
     virtual void set_size(Vector2) = 0;
    [[nodiscard]] virtual Vector2 get_size() const = 0;
    // cannot be const as it calls the events
    virtual void draw(Vector2 parent_pos) = 0;
};

}
