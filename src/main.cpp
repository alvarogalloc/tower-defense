#include <ios>
#include <nlohmann/json.hpp>
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

        fmt::println("assertion failed: {} (at {}:{})", message, loc.file_name(), loc.line());
        throw std::runtime_error("assertion failed");
    }
}

auto sprite_center(const sf::Sprite &sprite)
{
    return sf::Vector2f{sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2};
}

auto load_frames_json(const nlohmann::json &parent_node, std::string_view name)
{
    my_assert(parent_node[name].is_array());
    std::vector<sf::IntRect> frames;
    for (auto &&rect : parent_node[name])
    {
        auto res = frames.emplace_back(rect[0], rect[1], rect[2], rect[3]);
    }
    return frames;
}

constexpr std::vector<sf::Vector2f> get_turning_points(tmx_layer *layer_head)
{
    my_assert(layer_head != nullptr, "no layers found");
    while (layer_head)
    {
        if (layer_head->type == L_OBJGR)
        {
            break;
        }
        layer_head = layer_head->next;
    }
    my_assert(layer_head != nullptr, "no object layer found");

    std::vector<sf::Vector2f> points;
    auto current_object = layer_head->content.objgr->head;
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
    float frame_duration_seconds;
    std::size_t current_frame = 0;
    float acc_time_seconds = 0;
    Animation(std::vector<sf::IntRect> frames, sf::Texture &text, float frame_duration)
        : frame_rects(frames), frame_duration_seconds(frame_duration)
    {
        this->setTexture(text);
        this->setTextureRect(frame_rects.at(current_frame));
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
    const float velocity = 200.f;

    // get unit vector pointing to the next point
    my_assert(current_goal_point < directions.size(), "current goal point is out of bounds");
    auto normalize = [](sf::Vector2f vec) -> sf::Vector2f {
        float length = std::sqrt(std::pow(vec.x, 2) + std::pow(vec.y, 2));
        return {vec.x / length, vec.y / length};
    };
    const auto center = sprite_center(zombie);
    float distance = std::sqrt(std::pow(directions[current_goal_point].x - center.x - zombie.getPosition().x, 2) +
                               std::pow(directions[current_goal_point].y - center.y - zombie.getPosition().y, 2));
    if (distance < 3)
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
    auto move_vector = normalize(directions[current_goal_point] - center - zombie.getPosition()) * velocity;
    zombie.move(move_vector * delta);
}

void show_health_bar(const Animation &enemy);

int main(int argc, char *argv[])
{
    sf::RenderWindow win{sf::VideoMode{win_size.x, win_size.y}, "window"};
    const auto middle_screen = sf::VideoMode::getDesktopMode().width / 4;
    win.setPosition({static_cast<int>(middle_screen), 0});
    win.setFramerateLimit(60);
    win.setKeyRepeatEnabled(false);

    assets manager{"./assets/tiled"};
    auto text = manager.get<sf::Texture>("../characters.png");
    Tilemap map{"./assets/tiled/2.tmx", manager};
    auto enemies_turning_points = get_turning_points(map.get_map()->ly_head);
    std::ifstream anim_frames_file{"./assets/frames.json"};
    auto frames_json = nlohmann::json::parse(anim_frames_file);
    auto demon_frames = load_frames_json(frames_json, "big_demon_idle_anim");
    auto zombie_frames = load_frames_json(frames_json, "lizard_f_run_anim");

    std::vector<std::pair<Animation, std::size_t>> zombies;
    std::vector<std::pair<Animation, std::size_t>> demons;

    sf::Font f;
    if (!f.loadFromFile(font_path))
        throw std::runtime_error{"failed to load font"};

    sf::Event ev;
    sf::Clock clock;
    float delta = 0;
    while (win.isOpen())
    {
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                win.close();
            }
            if (ev.type == sf::Event::KeyPressed)
            {
                if (ev.key.code == sf::Keyboard::S)
                {
                    auto &[zombie, _] = zombies.emplace_back(Animation{zombie_frames, *text, 0.1f}, 1);
                    zombie.setScale(1.5, 1.5f);
                    zombie.setPosition(enemies_turning_points.front() - sprite_center(zombie));
                }
                else if (ev.key.code == sf::Keyboard::D)
                {
                    auto &[demon, _] = demons.emplace_back(Animation{demon_frames, *text, 0.1f}, 1);
                    demon.setScale(1.5, 1.5f);
                    demon.setPosition(enemies_turning_points.front() - sprite_center(demon));
                }
            }
        }
        delta = clock.restart().asSeconds();
        for (auto &&[zombie, goal] : zombies)
        {
            zombie.update(delta);
            update_zombie(zombie, enemies_turning_points, delta, goal);
        }
        for (auto &&[demon, goal] : demons)
        {
            demon.update(delta);
            update_zombie(demon, enemies_turning_points, delta, goal);
        }

        win.clear();
        win.draw(map);
        for (auto &&[demon, _] : demons)
        {
            // draw bounding box
            sf::RectangleShape rect;
            rect.setSize({demon.getGlobalBounds().width, demon.getGlobalBounds().height});
            rect.setPosition(demon.getPosition());
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(sf::Color::Red);
            rect.setOutlineThickness(2);

            win.draw(demon);
            win.draw(rect);
        }

        for (auto &&[zombie, _] : zombies)
        {
            // draw bounding box
            sf::RectangleShape rect;
            rect.setSize({zombie.getGlobalBounds().width, zombie.getGlobalBounds().height});
            rect.setPosition(zombie.getPosition());
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(sf::Color::Blue);
            rect.setOutlineThickness(2);

            win.draw(zombie);
            win.draw(rect);
        }
        win.display();
    }
}
