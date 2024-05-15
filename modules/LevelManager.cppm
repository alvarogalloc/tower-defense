export module LevelManager;
import stdbridge;
import sfml;
import tilemap;
import json;
import utils;

export
{
    enum class enemy_type { zombie, demon };

    class LevelManager
    {
        class Level
        {
            struct spawn_data
            {
                enemy_type type;
                std::size_t count;
                float time_for_next_wave;
            };

          public:
            // static Level from_json(const nlohmann::json& json)
            // {
            // }


            std::vector<sf::IntRect> m_spawning_zones;
            Tilemap m_map;
            std::vector<spawn_data> m_enemy_queue;
            float timer = 0;
        };

      public:
        LevelManager(std::string_view levels_file)
        {
            std::ifstream file(levels_file);
            if (!file.is_open())
            {
                my_assert(file.is_open(), "failed to open frames file");
            }
            m_levels_data = nlohmann::json::parse(file);
        }


      private:
        nlohmann::json m_levels_data;
    };
};
