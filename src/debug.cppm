export module debug;
import std;
import fmt;

export namespace debug {
  constexpr auto warn = fmt::emphasis::bold | fmt::fg(fmt::color::yellow);
  constexpr auto error = fmt::emphasis::bold | fmt::fg(fmt::color::red);
  constexpr auto info = fmt::emphasis::bold | fmt::fg(fmt::color::cyan);
  constexpr auto success = fmt::emphasis::bold | fmt::fg(fmt::color::green);

  constexpr void my_assert(
    const bool condition, std::string_view message,
    std::source_location loc = std::source_location::current())
  {
    if (!condition) {
      auto msg
        = fmt::format("assertion failed: {} (at {}:{})", message, loc.file_name(), loc.line());

      throw std::runtime_error(msg);
    }
  }
} // namespace debug
