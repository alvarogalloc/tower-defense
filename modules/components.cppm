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
  std::vector<sf::IntRect> frames;
  Animation(std::vector<sf::IntRect> frames,
    sf::Texture &text,
    float frame_duration)
    : frame_duration_seconds(frame_duration), frames(frames)
  {
    this->setTexture(text);
    this->setTextureRect(frames.at(current_frame));
  }
  void update(const float delta)
  {
    acc_time_seconds += delta;

    if (acc_time_seconds >= frame_duration_seconds)
    {
      if (current_frame == frames.size() - 1)
      {
        current_frame = 0;
      } else
      {
        // intentional, is more readable
        ++current_frame;
      }
      this->setTextureRect(frames.at(current_frame));
      acc_time_seconds = 0;
    }
  }
};

struct Health
{
  float max_health;
  Health(float max_health) : max_health(max_health), health(max_health) {}
  float health;
  void draw(sf::RenderTarget &target, const Animation &anim) const
  {
    sf::RectangleShape outsiderect;
    outsiderect.setSize({ anim.getGlobalBounds().width, 10 });
    outsiderect.setPosition(anim.getPosition().x, anim.getPosition().y - 10);
    outsiderect.setFillColor(sf::Color{ 0x323232aa });
    target.draw(outsiderect);
    sf::RectangleShape innerrect;
    innerrect.setSize(
      { (anim.getGlobalBounds().width - 4) * (health / max_health), 10 - 4 });
    innerrect.setPosition(anim.getPosition().x + 2, anim.getPosition().y - 8);
    innerrect.setFillColor(sf::Color::Green);
    target.draw(innerrect);
  }
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

enum class enemy_type { none = 0, zombie, demon };
constexpr std::string_view to_string(enemy_type type)
{
  using enum enemy_type;
  switch (type)
  {
  case zombie:
    return "zombie";
  case demon:
    return "demon";
  case none:
    return "none";
  }
}

enum class shooter_type { none = 0, wizard, knight };
constexpr std::string_view to_string(shooter_type type)
{
  using enum shooter_type;
  switch (type)
  {
  case wizard:
    return "wizard";
  case knight:
    return "knight";
  case none:
    return "none";
  }
}

}// namespace components
