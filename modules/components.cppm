export module components;
import sfml;
import stdbridge;
import json;
import utils;
export import ginseng;


export namespace components {
struct Animation : public sf::Sprite
{
    float frame_duration_seconds;
    std::size_t current_frame = 0;
    float acc_time_seconds = 0;
    std::vector<sf::IntRect> *frames;
    Animation(std::vector<sf::IntRect> *frames,
      sf::Texture &text,
      float frame_duration)
      : frame_duration_seconds(frame_duration), frames(frames)
    {
        this->setTexture(text);
        this->setTextureRect(frames->at(current_frame));
    }
    void update(const float delta)
    {
        acc_time_seconds += delta;

        if (acc_time_seconds >= frame_duration_seconds)
        {
            if (current_frame == frames->size() - 1)
            {
                current_frame = 0;
            } else
            {
                // intentional, is more readable
                ++current_frame;
            }
            this->setTextureRect(frames->at(current_frame));
            acc_time_seconds = 0;
        }
    }
};

struct Health
{
    float max_health;
    float health;
};

struct Velocity
{
    // velocity is the same always in xy
    float magnitude;
};

using draw_bounding_box = ginseng::tag<struct draw_bounding_box_t>;

struct BoundingBox : public sf::RectangleShape
{
    BoundingBox(const with_bounds auto &sprite)
    {
        setSize(
          { sprite.getGlobalBounds().width, sprite.getGlobalBounds().height });
        setPosition(sprite.getPosition() - sprite.getOrigin());
        setFillColor(sf::Color::Transparent);
        setOutlineColor(sf::Color::Red);
        setOutlineThickness(2);
    }
};


struct EnemyPath
{
    std::size_t current_point = 0;
};

struct Projectile : public sf::RectangleShape
{
    sf::Vector2f direction;
    float speed;
    float damage;
};
using player_tag = ginseng::tag<struct player_tag_t>;

enum class enemy_type {none,  zombie, demon };

}// namespace components
