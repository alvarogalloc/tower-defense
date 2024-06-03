export module LevelManager;
import stdbridge;
import sfml;
import tilemap;
import json;
import utils;
import components;
import EntityFactory;

export
{

  class LevelManager
  {
    class Level
    {
      struct spawn_data
      {
        components::enemy_type type;
        std::size_t count;
        float time_for_next_wave;
        float delay_each;
      };

    public:
      Level(const nlohmann::json &json);
      components::enemy_type update(float delta);
      std::string m_name;
      std::string m_tilemap_name;
      std::vector<sf::IntRect> m_spawning_zones;
      std::vector<spawn_data> m_waves;
      float timer = 0;
    };

  public:
    LevelManager(std::string_view levels_file, EntityFactory &factory);
    components::enemy_type update(float delta);
    auto &get_current_level() { return *m_current_level; }

  private:
    void load_levels();


    nlohmann::json m_levels_data;
    EntityFactory &m_factory;
    std::vector<Level> m_levels;
    std::unordered_map<std::string, Tilemap> maps;
    Level *m_current_level;
  };
};
