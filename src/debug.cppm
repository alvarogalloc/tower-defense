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
  struct message {
    float lifetime_seconds;
    std::string text;
    Color color;
  };
  
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

  void draw_debug( std::vector<message>& messages)
  {
    int start_y {10};
    int line_height {20};
    for (int i  = 0;auto& message : messages) {
      if (message.lifetime_seconds > 0.f) {
        message.lifetime_seconds -= GetFrameTime();
        DrawText(
          message.text.c_str(), 10, start_y + line_height * i, 20, message.color);
      }
      ++i;
    }
    auto removed = std::ranges::remove_if (messages,[](const auto& msg) { return msg.lifetime_seconds <= 0.f; });
    messages.erase(removed.begin(), messages.end());
  }
} // namespace debug
