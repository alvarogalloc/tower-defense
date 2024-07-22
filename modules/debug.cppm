export module debug;
import stdbridge;
import fmt;

export
{

  constexpr auto warn = fmt::emphasis::bold | fmt::fg(fmt::color::yellow);
  constexpr auto error = fmt::emphasis::bold | fmt::fg(fmt::color::red);
  constexpr auto info = fmt::emphasis::bold | fmt::fg(fmt::color::cyan);
  constexpr auto success = fmt::emphasis::bold | fmt::fg(fmt::color::green);


  void my_assert(bool condition,
    std::string_view message,
    std::source_location loc = std::source_location::current());
}