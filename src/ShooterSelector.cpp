module ShooterSelector;
import fmt;

using namespace components;
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
void ShooterSelector::deselect() { m_current_type = shooter_type::none; }


void ShooterSelector::add_option(sf::Color color,
  shooter_type type,
  const sf::Texture &text,
  const sf::IntRect &rect)

{
  if (ShooterSelector::option_size * m_options.size() > win_size.x)
  {
    fmt::print("the shooter selector is full!");
    return;
  }
  for (const auto &[_, opt_type, _s] : m_options)
  {

    if (type == opt_type)
    {
       fmt::print("the shooter is already in the selector!");
      return;
    }
  }
  auto &res = m_options.emplace_back(color, type, sf::Sprite(text, rect));

  sf::Vector2f pos{ option_size * (m_options.size() - 1), 640 };
  res.m_sprite.setPosition(pos);
  auto cal_scale = ShooterSelector::option_size / rect.height;
  res.m_sprite.setScale(cal_scale, cal_scale);
}

std::optional<ShooterSelector::shooter_spawn> ShooterSelector::choose_or_place(
  sf::Vector2f mouse_pos)
{
  if (mouse_pos.y > (win_size.y - 64.f))
  {
    // mouse in the selector, we will choose
    // the shooter
    for (const auto &option : m_options)
    {
      sf::FloatRect rect{ option.m_sprite.getPosition(),
        sf::Vector2f{ option_size, option_size } };
      if (rect.contains(mouse_pos))
      {
        m_current_type = option.m_type;
      } else
      {
        m_current_type = shooter_type::none;
      }
    }
    return {};
  }

  // mouse in the game area, we will place it
  if (m_current_type == shooter_type::none) { return {}; }
  sf::Vector2i tile_coords{ static_cast<int>(mouse_pos.x / 32.f),
    static_cast<int>(mouse_pos.y / 32.f) };
  return ShooterSelector::shooter_spawn{ m_current_type,
    sf::Vector2f{ tile_coords.x * 32.f, tile_coords.y * 32.f } };
}
