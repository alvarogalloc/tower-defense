module;
#include <cstdlib>
module config;

namespace config {

config::config() {
  //  load game dir
  if (m_game_dir.empty()) {
    char* env_value = std::getenv("MAGSTER_DIR");
    debug::my_assert(
        env_value, "MAGSTER_DIR is empty, it should be set in the environment");
    m_game_dir = env_value;
    if (m_game_dir.back() != '/') m_game_dir.push_back('/');
  }

  // buffer for glaze
  std::string buf;
  using namespace glz;

  // load app config
  const auto app_config{get_game_dir() + "/assets/game_info.json"};
  auto ec = read_file_json(m_app_info, app_config, buf);

  const auto check_load_config = [&] {
    if (ec) {
      debug::my_assert(false,
                       std::format("Error parsing configuration file: {}",
                                   format_error(ec, buf)));
    } else {
      std::println("Configuration loaded successfully.");
    }
  };

  check_load_config();

  // load game config
  //
  const std::string config_file_path = get_path("/assets/game_config.json");
  std::println("Loading game config from {}", config_file_path);

  ec = read_file_json(m_game_config, config_file_path, buf);
  check_load_config();
}
auto config::get_path(const std::string_view p) const -> std::string {
  const auto s = get_game_dir() + p;
  namespace fs = std::filesystem;
  debug::my_assert(fs::exists(fs::path{s}),
                   std::format("path {} not found", s));

  return s;
}

}  // namespace config
