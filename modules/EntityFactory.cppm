export module EntityFactory;
import stdbridge;
import components;
import sfml;
import assets;
import ginseng;


export
{
  class EntityFactory
  {
    using ent_id = ginseng::database::ent_id;

  public:
    EntityFactory(my_assets &manager, std::string_view frames_path);

    void set_enemy_turning_points(std::vector<sf::Vector2f> &&points);

    ent_id spawn_enemy(ginseng::database &db, components::enemy_type type);

    ent_id spawn_zombie(ginseng::database &db);

    ent_id spawn_demon(ginseng::database &db);

    ent_id spawn_shooter(ginseng::database &db, components::shooter_type type, sf::Vector2f position);
    ent_id spawn_wizard(ginseng::database &db, sf::Vector2f position);
    ent_id spawn_knight(ginseng::database &db, sf::Vector2f position);

    auto get_zombie_frames() const { return zombie_frames; }
    auto get_demon_frames() const { return demon_frames; }
    auto get_wizard_frames() const { return wizard_frames; }
    auto get_knight_frames() const { return knight_frames; }
    auto get_enemy_turning_points() const { return enemies_turning_points; }

  private:
    my_assets *manager;
    sf::Texture *texture;

    // frames of different kinds of enemies, right now only demon and zombie
    std::vector<sf::IntRect> demon_frames;
    std::vector<sf::IntRect> wizard_frames;
    std::vector<sf::IntRect> zombie_frames;
    std::vector<sf::IntRect> knight_frames;

    sf::Vector2f wizard_origin;
    std::vector<sf::Vector2f> enemies_turning_points;
  };
}
