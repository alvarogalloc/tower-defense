export module shooter_factory;
import stdbridge;
import components;
import ginseng;
import sfml;
import json;
import assets;


export
{

  class shooter_factory
  {
  public:
    shooter_factory(sf::Texture* texture, const nlohmann::json& node);

    struct shooter_info
    {
      std::string_view name;
      components::shooter_type type;
      std::uint8_t damage;
      std::vector<sf::IntRect> frames;
    };

    shooter_info get_shooter_info(components::shooter_type type) const
    {
      return m_shooter_data.at(type);
    }
    auto get_shooter_data() const { return m_shooter_data; }

    using ent_id = ginseng::database::ent_id;
    ent_id spawn_shooter(ginseng::database &db,
      components::shooter_type type,
      sf::Vector2f position);

  private:
    std::unordered_map<components::shooter_type, shooter_info> m_shooter_data;
    sf::Texture *m_texture;
  };
}

