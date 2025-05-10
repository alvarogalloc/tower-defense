module gui.container;

namespace gui::v2
{

void container::draw() const
{
  if (children.size() == 0)return;
    auto pos = this->position;

    if (this->alflags == align_flags::end)
    {
      pos.x+= this->size.x;
    }



    for (const auto &child : this->children)
    {
        // log warning when out of bounds float checked_dimension = dir ==
        // irection::horizontal ? pos
        //     x : pos.y; t dimension_limit =
        //     dir == direction::horizontal ? this->size.x : this->size.y;
        // if (checked_dimension > dimension_limit)
        // { std::cerr << "Child widget exceeds container bounds!" << s
        //     d::endl;
        //     return;
        // }

        if (this->dir == direction::horizontal)
        {
            if (this->alflags == align_flags::end)
                pos.x -= child->get_size().x;
        }
        else
        {
            if (this->alflags == align_flags::end)
                pos.y -= child->get_size().y;
        }
        child->draw(pos);
        if (this->dir == direction::horizontal)
        {
            if (this->alflags == align_flags::start)
                pos.x += child->get_size().x;
        }
        else
        {
            if (this->alflags == align_flags::start)
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
