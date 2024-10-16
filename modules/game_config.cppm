
export module game_config;
import std;
import debug;
import fmt;
import raylib;

struct game_config {
  bool valid = false;
  Color bg_color;
};

constexpr static std::string_view config_file_path = SRC_DIR "/assets/game_config.txt";

export game_config get_game_config()
{
  static game_config gc;
  if (!gc.valid) {
    fmt::print(info, "loading config file from {}\n", config_file_path);
    // load only once
    std::ifstream file {config_file_path.data()};
    my_assert(file.is_open(), "could not open config file");
    std::string line;
    while (std::getline(file, line)) {
      std::istringstream iss(line);
      std::string property;
      iss >> property;
      if (property == "bg_color") {
        unsigned int hex_color = 0x00;
        iss >> std::hex >> hex_color; // read hex value
        gc.bg_color = GetColor(hex_color);
      }
    }
    gc.valid = true;
  }
  return gc;
}
