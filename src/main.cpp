#include <ios>
import sfml;
import fmt;
import stdutils;
import tmx;
import assets;
import stringables;
import tilemap;

const sf::Vector2u win_size{640, 640};

constexpr auto font_path = "/System/Library/Fonts/Times.ttc";

struct Animation : public sf::Drawable
{
    std::vector<sf::IntRect> frame_rects;
    sf::Sprite sprite;
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
            fmt::println("name: {}, rect: {}", name, to_string(rect));
        }
        sprite.setTexture(text);
        sprite.setTextureRect(frame_rects[current_frame]);
    }
    void update(const float delta)
    {
        acc_time_seconds += delta;
        if (acc_time_seconds >= frame_duration_seconds)
        {
            if (current_frame == frame_rects.size())
            {
            }
            sprite.setTextureRect(frame_rects[++current_frame]);
        }
    }
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(sprite, states);
    }
};

int main(int argc, char *argv[])
{
    sf::RenderWindow win{sf::VideoMode{win_size.x, win_size.y}, "window"};
    std::ifstream file{"./assets/characters-tiles.txt"};

    const auto middle_screen = sf::VideoMode::getDesktopMode().width / 4;
    win.setPosition({static_cast<int>(middle_screen), 0});

    win.setFramerateLimit(60);

    assets manager{"./assets/tiled"};
    auto text = manager.get<sf::Texture>("../characters.png");
    Animation anim{"big_zombie_run_anim", file, *text, 1};
    Tilemap map{"./assets/tiled/1.tmx", manager};

    sf::Font f;
    if (!f.loadFromFile(font_path))
        throw;

    sf::Event ev;
    sf::Clock clock;
    while (win.isOpen())
    {
        while (win.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
            {
                win.close();
            }
        }
        anim.update(clock.restart().asSeconds());
        win.clear();
        win.draw(map);
        win.draw(anim);
        win.display();
    }
}
