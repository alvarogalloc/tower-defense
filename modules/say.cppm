module;
#include <fmt/color.h>
#include <fmt/core.h>
#include <string_view>

export module say;

void _log(const fmt::color color, const std::string_view message)
{
    fmt::print(fmt::emphasis::bold | fg(color), "{}\n", message);
}
export namespace say {

void debug([[maybe_unused]] std::string_view message)
{
#ifndef NDEBUG
    _log(
      fmt::color::gold, fmt::format("[DEBUG]: {}", message));
#endif// NDEBUG
}
void info(std::string_view message)
{
    _log(fmt::color::royal_blue, fmt::format("[INFO]: {}", message));
}
void warn(std::string_view message)
{
    _log(fmt::color::orange, fmt::format("[WARN]: {}", message));
}
void error(std::string_view message)
{
    _log(fmt::color::red, fmt::format("[ERROR]: {}", message));
}
}// namespace say

export namespace fmt {
using ::fmt::format;
using ::fmt::print;
using ::fmt::println;
}// namespace fmt
