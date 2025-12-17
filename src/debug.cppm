export module debug;
import std;
import raylib;
export namespace debug {
namespace colors {

constexpr inline std::string_view bold = "\033[1m";
constexpr inline std::string_view yellow = "\033[33m";
constexpr inline std::string_view red = "\033[31m";
constexpr inline std::string_view cyan = "\033[36m";
constexpr inline std::string_view green = "\033[32m";

}  // namespace colors

struct message {
  float lifetime_seconds;
  std::string text;
};

struct assert_error : std::exception {
    explicit assert_error(std::string_view message) : msg(std::format("\n    ASSERT ERROR: {}", message)) {}

        // Override the what() method to return the custom error message
        [[nodiscard("this is an error bro, you definitely need it")]]
        const char* what() const noexcept override {
            return msg.c_str();
        }

    private:
        std::string msg;

};

constexpr std::string_view reset = "\033[0m";
const std::string warn = std::format("{}{}", colors::bold, colors::yellow);
const std::string error = std::format("{}{}", colors::bold, colors::red);
const std::string info = std::format("{}{}", colors::bold, colors::cyan);
const std::string success = std::format("{}{}", colors::bold, colors::green);

constexpr void my_assert(
    const bool condition, std::string_view message,
    std::source_location loc = std::source_location::current()) {
  if (!condition) {
    auto msg = std::format("assertion failed: {} (at {}:{})", message,
                           loc.file_name(), loc.line());

    throw std::runtime_error(msg);
  }
}
}  // namespace debug
