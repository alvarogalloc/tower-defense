export module debug;
import std;
import ginseng;
import raylib;
export namespace debug
{
namespace colors
{

constexpr inline std::string_view bold = "\033[1m";
constexpr inline std::string_view yellow = "\033[33m";
constexpr inline std::string_view red = "\033[31m";
constexpr inline std::string_view cyan = "\033[36m";
constexpr inline std::string_view green = "\033[32m";

} // namespace colors

struct message
{
    float lifetime_seconds;
    std::string text;
    Color color;
};

constexpr std::string_view reset = "\033[0m";
const std::string warn =
    std::format("{}{}{}", colors::bold, colors::yellow, "{}");
const std::string error =
    std::format("{}{}{}", colors::bold, colors::red, "{}");
const std::string info =
    std::format("{}{}{}", colors::bold, colors::cyan, "{}");
const std::string success =
    std::format("{}{}{}", colors::bold, colors::green, "{}");

constexpr void my_assert(
    const bool condition, std::string_view message,
    std::source_location loc = std::source_location::current())
{
    if (!condition)
    {
        auto msg = std::format("{}assertion failed: {} (at {}:{}){}", error,
                               message, loc.file_name(), loc.line(), reset);

        throw std::runtime_error(msg);
    }
}

void draw_debug(std::vector<message> &messages)
{
    const int start_y{10};
    const int pos_x {10};
    const int line_height{20};
    const int font_size{20};
    for (int i = 0; auto &message : messages)
    {
        if (message.lifetime_seconds > 0.f)
        {
            message.lifetime_seconds -= GetFrameTime();
            DrawText(message.text.c_str(), pos_x, start_y + line_height * i, font_size,
                     message.color);
        }
        ++i;
    }
    auto removed = std::ranges::remove_if(
        messages, [](const auto &msg) { return msg.lifetime_seconds <= 0.f; });
    messages.erase(removed.begin(), messages.end());
}
} // namespace debug
