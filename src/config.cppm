export module config;
import json;
import std;
import fmt;
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
    std::println("Loading game config from {}", config_file_path);
    std::ifstream f(config_file_path.data());
    std::string contents((std::istreambuf_iterator<char>(f)),
                         (std::istreambuf_iterator<char>()));
    data = Document();
    data.Parse(contents.c_str());
#endif
    return data;
}

struct app_info
{
    std::string_view window_name;
    std::string asset_path;
    Vector2 size;
    int fps;
};

#if __has_cpp_attribute(__cpp_lib_constexpr_)
constexpr
#else
const
#endif
    app_info game_info{

        .window_name = "Hello, World!",
        .asset_path = SRC_DIR "/assets",
        .size = {1280, 720},
        .fps = 60,
    };
} // namespace config
