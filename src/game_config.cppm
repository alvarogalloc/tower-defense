export module game_config;
import json;
import std;
import fmt;

constexpr static std::string_view config_file_path = SRC_DIR "/assets/game_config.json";

export auto get_game_config()
{
  using nlohmann::json;
  static std::optional<json> data;
  if (!data.has_value()) {
    fmt::println("Loading game config from {}", config_file_path);
    std::ifstream f(config_file_path.data());
    data = json::parse(f);
  }
  return data.value();
}
