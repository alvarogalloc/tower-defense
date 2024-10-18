export module EnemyFactory;

import stdbridge;
import components;
import sfml;
import ginseng;
import json;

// this class should only hold the info to spawn enemies
// this is name, type, health, speed and frames
// it should not be responsible for adding enemies to the scene
class my_assets;
export class EnemyFactory
{
  struct enemy_data
  {
    components::enemy_type type;
    float health;
    float speed;
    std::vector<sf::IntRect> frames;
  };
  using ent_id = ginseng::database::ent_id;

public:
  // the json should be an array of objects with the following fields
  // name, type, health, speed, frames(idle is not optional, the rest are for
  // now ignored)
  EnemyFactory(my_assets &manager, const nlohmann::json &enemies_json);
  ent_id spawn_enemy(ginseng::database &db,
    components::enemy_type type,
    sf::Vector2f spawn_point);
  auto get_enemy_data(components::enemy_type type) const
  {
    return m_enemy_data.at(type);
  }

private:
  std::unordered_map<components::enemy_type, enemy_data> m_enemy_data;
  sf::Texture *m_frames_texture;
};

/*
class EntityFactory
  {
    using ent_id = ginseng::database::ent_id;

  public:
    EntityFactory(my_assets &manager, std::string_view frames_path);

    void set_enemy_turning_points(std::vector<sf::Vector2f> &&points);

    ent_id spawn_enemy(ginseng::database &db, components::enemy_type type);

    ent_id spawn_zombie(ginseng::database &db);

    ent_id spawn_demon(ginseng::database &db);

    ent_id spawn_shooter(ginseng::database &db, components::shooter_type type,
sf::Vector2f position); ent_id spawn_wizard(ginseng::database &db, sf::Vector2f
position); ent_id spawn_knight(ginseng::database &db, sf::Vector2f position);

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
*/
