export module config;
import json;
import std;
import raylib;

export namespace config
{
rapidjson::Document &get_game_config()
{
    constexpr static std::string_view config_file_path =
        SRC_DIR "/assets/game_config.json";
#if 0
    using nlohmann::json;
    static std::optional<json> data;
    if (!data.has_value())
    {
        std::println("Loading game config from {}", config_file_path);
        std::ifstream f(config_file_path.data());
        data = json::parse(f);
    }
#else
    using namespace rapidjson;
    static Document data;
    if (data.IsNull())
    {
        std::println("Loading game config from {}", config_file_path);
        std::ifstream f(config_file_path.data());
        std::string contents((std::istreambuf_iterator<char>(f)),
                             (std::istreambuf_iterator<char>()));
        data = Document();
        data.Parse(contents.c_str());
    }
#endif
    return data;
}

struct app_info
{
    std::string_view window_name;
    std::string_view asset_path;
    Vector2 size;
    Vector2 game_res;
    int fps;
};

constexpr inline app_info game_info{
    .window_name = "Hello, World!",
    .asset_path = SRC_DIR "/assets",
    .size {640*2, 360*2},
    .game_res{640, 360},
    .fps = 30,
};

} // namespace config
