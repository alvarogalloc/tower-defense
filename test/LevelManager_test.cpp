import boost.ut;
import utils;
import stdbridge;
import sfml;
import json;

int main(int argc, char *argv[])
{
    using namespace boost::ut;
    using namespace boost::ut::literals;

    "Test Level JSON Type Checks"_test = []() {
            nlohmann::json root;
    {
        std::ifstream file{ get_resource_path() + "../assets/levels.json" };
        root = nlohmann::json::parse(file);
        file.close();
    }

        for (const auto &level_json : root)
        {

            // Type checks for the "name" field
            expect(level_json.contains("name")) << "Missing 'name' field";
            expect(level_json["name"].is_string())
              << "'name' should be a string";

            // Type checks for the "tilemap" field
            expect(level_json.contains("tilemap")) << "Missing 'tilemap' field";
            expect(level_json["tilemap"].is_string())
              << "'tilemap' should be a string";

            // Type checks for the "spawningzones" field
            expect(level_json.contains("spawningzones"))
              << "Missing 'spawningzones' field";
            expect(level_json["spawningzones"].is_array())
              << "'spawningzones' should be an array";

            for (const auto &zone : level_json["spawningzones"])
            {
                expect(zone.is_array())
                  << "Each spawning zone should be an array";
                expect(zone.size() == 4)
                  << "Each spawning zone should have 4 integers";
                for (const auto &coord : zone)
                {
                    expect(coord.is_number_integer())
                      << "Spawning zone coordinates should be integers";
                }
            }

            // Type checks for the "waves" field
            expect(level_json.contains("waves")) << "Missing 'waves' field";
            expect(level_json["waves"].is_array())
              << "'waves' should be an array";

            for (const auto &wave : level_json["waves"])
            {
                expect(wave.contains("type")) << "Missing 'type' field in wave";
                expect(wave["type"].is_number_integer())
                  << "'type' should be an integer";

                expect(wave.contains("count"))
                  << "Missing 'count' field in wave";
                expect(wave["count"].is_number_integer())
                  << "'count' should be an integer";

                expect(wave.contains("time_for_next_wave"))
                  << "Missing 'time_for_next_wave' field in wave";
                expect(wave["time_for_next_wave"].is_number_float())
                  << "'time_for_next_wave' should be a float";
            }
        }
    };

}
