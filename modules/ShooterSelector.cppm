export module ShooterSelector;
import sfml;
import stdbridge;
import say;
import utils;

export
{
  // forward declaration
  enum class shooter_type;
  class ShooterSelector;

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


  class ShooterSelector : public sf::Drawable
  {
    constexpr static float option_size{ 64.f };

    struct shooter_option : public sf::Drawable
    {
      shooter_option(sf::Color color, shooter_type type, sf::Sprite sprite);
      void draw(sf::RenderTarget &target,
        sf::RenderStates states) const override;
      sf::Color m_color;
      shooter_type m_type;
      sf::Sprite m_sprite;
    };

  public:
    ShooterSelector(sf::Font *font);
    auto get_current_type() const { return m_current_type; }
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void add_option(sf::Color color,
      shooter_type type,
      const sf::Texture &text,
      const sf::IntRect &rect);
    void choose(sf::Vector2f mouse_pos);


  private:
    std::vector<shooter_option> m_options;
    sf::Font *m_font;
    shooter_type m_current_type = shooter_type::none;
  };
}
module :private;

ShooterSelector::shooter_option::shooter_option(sf::Color color,
  shooter_type type,
  sf::Sprite sprite)
  : m_color(color), m_type(type), m_sprite(sprite)
{}

void ShooterSelector::shooter_option::draw(sf::RenderTarget &target,
  sf::RenderStates states) const
{
  sf::RectangleShape shooter_selector_bg;
  shooter_selector_bg.setSize(
    { ShooterSelector::option_size, ShooterSelector::option_size });
  shooter_selector_bg.setPosition(m_sprite.getPosition());
  shooter_selector_bg.setFillColor(m_color);
  target.draw(shooter_selector_bg, states);
  target.draw(m_sprite, states);
}


ShooterSelector::ShooterSelector(sf::Font *font) : m_font(font) {}

void ShooterSelector::draw(sf::RenderTarget &target,
  sf::RenderStates states) const
{
  for (auto option : m_options)
  {
    target.draw(option, states);
    if (option.m_type == m_current_type)
    {
      sf::RectangleShape shape;
      shape.setFillColor(sf::Color{ 0x00000055 });
      shape.setPosition(option.m_sprite.getPosition());
      shape.setSize(sf::Vector2f{ option_size, option_size });
      target.draw(shape, states);
    }
  }
}

void ShooterSelector::add_option(sf::Color color,
  shooter_type type,
  const sf::Texture &text,
  const sf::IntRect &rect)

{
  if (ShooterSelector::option_size * m_options.size() > win_size.x)
  {
    say::warn("the shooter selector is full!");
    return;
  }
  for (const auto &[_, opt_type, _s] : m_options)
  {

    if (type == opt_type)
    {
      say::warn("the shooter is already in the selector!");
      return;
    }
  }
  auto &res = m_options.emplace_back(color, type, sf::Sprite(text, rect));

  sf::Vector2f pos{ option_size * (m_options.size() - 1), 640 };
  res.m_sprite.setPosition(pos);
  auto cal_scale = ShooterSelector::option_size / rect.height;
  res.m_sprite.setScale(cal_scale, cal_scale);
}


void ShooterSelector::choose(sf::Vector2f mouse_pos)
{
  if (mouse_pos.y < (win_size.y - 120.f))
  {
    m_current_type = shooter_type::none;
  }

  for (const auto &option : m_options)
  {
    sf::FloatRect rect{ option.m_sprite.getPosition(),
      sf::Vector2f{ option_size, option_size } };
    if (rect.contains(mouse_pos))
    {
      m_current_type = option.m_type;
      return;
    } else
    {
      m_current_type = shooter_type::none;
    }
  }
}
