export module debug;
import std;
import ginseng;
import raylib;
constexpr static std::string_view bold = "\033[1m";

constexpr static std::string_view yellow = "\033[33m";
constexpr static std::string_view red = "\033[31m";
constexpr static std::string_view cyan = "\033[36m";
constexpr static std::string_view green = "\033[32m";

export namespace debug {

  constexpr std::string_view reset = "\033[0m";
  const std::string warn = std::format("{}{}{}", bold, yellow, "{}");
  const std::string error = std::format("{}{}{}", bold, red, "{}");
  const std::string info = std::format("{}{}{}", bold, cyan, "{}");
  const std::string success = std::format("{}{}{}", bold, green, "{}");

  constexpr void my_assert(
    const bool condition, std::string_view message,
    std::source_location loc = std::source_location::current())
  {
    if (!condition) {
      auto msg = std::format(
        "{}assertion failed: {} (at {}:{}){}", error, message, loc.file_name(), loc.line(), reset);

      throw std::runtime_error(msg);
    }
  }

  void draw_debug(ginseng::database& db)
  {
    DrawText("Debug mode", 10, 10, 20, colors::yellow);
    const std::string text = std::format("Entities: {}", db.size());
    DrawText(text.c_str(), 10, 30, 20, colors::yellow);
  }
} // namespace debug
