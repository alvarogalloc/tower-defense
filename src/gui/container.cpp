

module gui.container;
using namespace rooster;

namespace
{
template <class... Ts> struct overloaded : Ts...
{
    using Ts::operator()...;
};

} // namespace
namespace gui::v2
{

void container::draw(Vector2 parent_end) const
{
    if (children.size() == 0)
        return;

    auto start_pos = [&] {
        if (Vector2Equals(parent_end, Vector2{0, 0}))
            return position;
        else
        {
            // std::println("offset of container x {} y {}", parent_end.x,
            // parent_end.y);
            return parent_end;
        }
    }();

    if (dir == direction::horizontal)
    {

        Vector2 widgets_size = std::accumulate(
            children.begin(), children.end(), Vector2{0, 0},
            [this](auto acc, auto widget) {
                return acc +
                       std::visit([](auto el) { return el->get_size(); },
                                  widget) +
                       Vector2{gap, gap};
            });
        if (alflags == h_alignment::center)
        {
            start_pos.x += get_size().x / 2 - widgets_size.x / 2;
        }
        else if (alflags == h_alignment::right)
        {

            start_pos.x += get_size().x - widgets_size.x;
        }
    }

    auto draw_elements = [&](auto view) {
        Vector2 acc_offset = start_pos;
        for (const auto &el : view)
        {
            std::visit(
                overloaded{
                    [&](auto w) {
                        Vector2 pos = {0, 0};
                        if (dir == direction::vertical)
                        {
                            using enum h_alignment;
                            pos.y = acc_offset.y;
                            switch (alflags)
                            {
                            case left:
                                pos.x = start_pos.x;
                                break;
                            case right:
                                pos.x = start_pos.x + get_size().x -
                                        w->get_size().x;
                                break;
                            case center:
                                pos.x = start_pos.x + (get_size().x / 2) -
                                        (w->get_size().x / 2);
                                break;
                            }
                        }
                        else if (dir == direction::horizontal)
                        {
                            pos.y = start_pos.y;
                            pos.x = acc_offset.x;
                        }

                        w->draw(pos);
                        acc_offset += w->get_size() + Vector2{gap, gap};
                    }
                    // [&](container::container_t cont) {
                    //     cont->draw(acc_offset);
                    //     acc_offset += cont->get_size() + Vector2{gap, gap};
                    // },

                },
                el);
        }
    };

    draw_elements(children);
}
void container::draw() const
{
    draw({0, 0});
}

void container::draw_box_model(Vector2 parent_end) const
{

    constexpr static double line_width = 1.0;
    Vector2 pos = this->position + parent_end;

    for (const auto &el : this->children)
    {
        std::visit(
            overloaded{
                [&](container::widget_t) {
                    // DrawRectangleLinesEx({pos.x, pos.y, w_x, w_y},
                    // line_width,
                    //                      colors::blue);
                },
                [&](container::container_t cont) { cont->draw_box_model(pos); },

            },
            el);
    }
    const auto container_pos = this->position + parent_end;
    DrawRectangleLinesEx(
        {container_pos.x, container_pos.y, this->size.x, this->size.y},
        line_width, colors::red);
}
void container::draw_box_model() const
{
    draw_box_model({0, 0});
}

} // namespace gui::v2
