export module ShooterSelector;
import sfml;
import stdbridge;
import utils;
import components;

export
{
  class ShooterSelector : public sf::Drawable
  {
    constexpr static float option_size{ 64.f };

    struct shooter_option : public sf::Drawable
    {
      shooter_option(sf::Color color,
        components::shooter_type type,
        sf::Sprite sprite);
      void draw(sf::RenderTarget &target,
        sf::RenderStates states) const override;
      sf::Color m_color;
      components::shooter_type m_type;
      sf::Sprite m_sprite;
    };
    struct shooter_spawn
    {
      components::shooter_type type;
      sf::Vector2f pos;
    };

  public:
    ShooterSelector(sf::Font *font);
    auto get_current_type() const { return m_current_type; }
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void deselect();
    void add_option(sf::Color color,
      components::shooter_type type,
      const sf::Texture &text,
      const sf::IntRect &rect);

    [[nodiscard]] std::optional<shooter_spawn> choose_or_place(
      sf::Vector2f mouse_pos);

  private:
    std::vector<shooter_option> m_options;
    sf::Font *m_font;
    components::shooter_type m_current_type = components::shooter_type::none;
  };
}
