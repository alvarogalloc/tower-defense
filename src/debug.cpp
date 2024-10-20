module debug;

namespace debug {

  void my_assert(bool condition, std::string_view message, std::source_location loc)
  {
    if (!condition) {
      auto msg
        = fmt::format("assertion failed: {} (at {}:{})", message, loc.file_name(), loc.line());
      throw std::runtime_error(msg);
    }
  }
} // namespace debug
