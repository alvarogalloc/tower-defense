export module LevelManager;
import tilemap;
import json;
import utils;
import sfml;
import components;
import stdbridge;
import EnemyFactory;
import assets;
import ginseng;

export
{

  class LevelManager: public sf::Drawable
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
      Level() = default;
      Level(const nlohmann::json &levelnode, my_assets &manager);
      [[nodiscard]] components::enemy_type update(float delta, bool draw_debug);


      std::string m_name;
      std::string m_tilemap_name;
      std::vector<spawn_data> m_waves;
      std::vector<sf::Vector2f> m_enemies_turning_points;
      float timer = 0;
      Tilemap m_map;
    };


  public:
    [[deprecated]] LevelManager(std::string_view levels_file,
      my_assets &manager);
    LevelManager(nlohmann::json levels_json,
      nlohmann::json enemies_json,
      my_assets &manager);
    // here should free last tilemap as no next_level will be called
    ~LevelManager();
     void update(ginseng::database &db, float delta);
    auto &get_current_level() { return *m_current_level; }
    void next_level();
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    void set_draw_debug(bool draw_debug) { m_draw_debug = draw_debug; }
    auto get_draw_debug() const { return m_draw_debug; }

  private:
    void load_levels();
    bool m_draw_debug = false;
    std::unique_ptr<Level> m_current_level;
    std::size_t m_current_level_index = 0;
    std::unique_ptr<EnemyFactory> m_enemy_factory;
    my_assets *m_manager;
    nlohmann::json m_levels_json;
  };
};
