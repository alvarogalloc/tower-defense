#include <algorithm>
#include <ios>
#include <source_location>
import sfml;
import fmt;
import stdutils;
import tmx;
import assets;
import stringables;
import tilemap;

const sf::Vector2u win_size{640, 640};

constexpr auto font_path = "/System/Library/Fonts/Times.ttc";

void my_assert(bool condition, std::string_view message = "",
               std::source_location loc = std::source_location::current())
{
    if (!condition)
    {

        fmt::print("assertion failed: {} at: ({}:{})", message, loc.file_name(), loc.line());
        throw std::runtime_error("assertion failed");
    }
}

constexpr std::vector<sf::Vector2f> get_turning_points(const tmx_layer *layer)
{
    std::vector<sf::Vector2f> points;
    auto current_object = layer->content.objgr->head;
    while (current_object)
    {
        points.emplace_back(sf::Vector2f{static_cast<float>(current_object->x), static_cast<float>(current_object->y)});
        current_object = current_object->next;
    }
    std::reverse(points.begin(), points.end());
    return points;
}

struct Animation : public sf::Sprite
{
    std::vector<sf::IntRect> frame_rects;
    std::size_t current_frame = 0;
    float acc_time_seconds = 0;
    float frame_duration_seconds;
    Animation(std::string_view t_name, std::ifstream &frames_file, sf::Texture &text, float frame_duration)
        : frame_duration_seconds(frame_duration)
    {
        if (!frames_file.is_open())
            throw;
        frames_file.seekg(std::ios_base::beg);
        while (!frames_file.eof())
        {
            // the order of the line is
            std::string name;
            int top, left, width, height;
            frames_file >> name;
            // fix when last is also counted
            if (!name.size())
                break;
            if (!name.contains(t_name))
                continue;

            frames_file >> top >> left >> width >> height;
            auto rect = frame_rects.emplace_back(top, left, width, height);
            fmt::println("loaded frame {} with rect ({})", name, to_string(rect));
        }
        this->setTexture(text);
        this->setTextureRect(frame_rects[current_frame]);
    }
    void update(const float delta)
    {
        acc_time_seconds += delta;

        if (acc_time_seconds >= frame_duration_seconds)
        {
            if (current_frame == frame_rects.size() - 1)
            {
                current_frame = 0;
            }
            else
            {
                // intentional, is more readable
                ++current_frame;
            }
            this->setTextureRect(frame_rects[current_frame]);
            acc_time_seconds = 0;
        }
    }
};

// returns the next point it is moving to
void update_zombie(Animation &zombie, std::span<sf::Vector2f> directions, const float delta,
                   std::size_t &current_goal_point)
{
    // move the zombie to the next point
    const float velocity = 100.f;

    // get unit vector pointing to the next point
    my_assert(current_goal_point < directions.size(), "current goal point is out of bounds");
    auto normalize = [](sf::Vector2f vec) -> sf::Vector2f {
        float length = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
        return {vec.x / length, vec.y / length};
    };
    float distance = std::sqrt(std::pow(directions[current_goal_point].x - zombie.getPosition().x, 2) +
                               std::pow(directions[current_goal_point].y - zombie.getPosition().y, 2));
    if (distance < 1)
    {
        if (current_goal_point == directions.size() - 1)
        {
            current_goal_point = 0;
        }
        else
        {
            ++current_goal_point;
        }
    }
    auto move_vector = normalize(directions[current_goal_point] - zombie.getPosition()) * velocity;
    zombie.move(move_vector * delta);
}

int main(int argc, char *argv[])
{
    sf::RenderWindow win{sf::VideoMode{win_size.x, win_size.y}, "window"};
    std::ifstream file{"./assets/characters-tiles.txt"};

    const auto middle_screen = sf::VideoMode::getDesktopMode().width / 4;
    win.setPosition({static_cast<int>(middle_screen), 0});

    win.setFramerateLimit(60);

    assets manager{"./assets/tiled"};
    auto text = manager.get<sf::Texture>("../characters.png");
    Animation zombie{"big_zombie_idle_anim", file, *text, 0.1f};
    zombie.setScale(1.8, 1.8);
    Tilemap map{"./assets/tiled/1.tmx", manager};

    sf::Font f;
    if (!f.loadFromFile(font_path))
        throw;

    const tmx_map *raw_map = map.get_map();
    // spawn the player to the first object
    auto *obj_layer = raw_map->ly_head;
    my_assert(obj_layer != nullptr, "no layers found");
    while (obj_layer)
    {
        if (obj_layer->type == L_OBJGR)
        {
            break;
        }
        obj_layer = obj_layer->next;
    }
    my_assert(obj_layer != nullptr, "no object layer found");

    auto turning_points = get_turning_points(obj_layer);
    for (auto &&point : turning_points)
    {
        fmt::print("turning point: {}\n", to_string(point));
    }
    // set center of the sprite
    zombie.setOrigin(zombie.getLocalBounds().width / 2, zombie.getLocalBounds().height / 2);
    zombie.setPosition(turning_points.front());

    sf::Event ev;
    sf::Clock clock;
    float delta = 0;
    std::size_t current_point = 1;
    while (win.isOpen())
    {
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                win.close();
            }
        }
        delta = clock.restart().asSeconds();
        zombie.update(delta);
        update_zombie(zombie, turning_points, delta, current_point);
        // draw bounding box
        sf::RectangleShape rect;
        rect.setSize({zombie.getLocalBounds().width, zombie.getLocalBounds().height});
        rect.setPosition(zombie.getPosition());
        rect.setFillColor(sf::Color::Transparent);
        rect.setOutlineColor(sf::Color::Blue);
        rect.setOutlineThickness(2);
        win.clear();
        win.draw(map);
        win.draw(rect);
        win.draw(zombie);
        win.display();
    }
}
