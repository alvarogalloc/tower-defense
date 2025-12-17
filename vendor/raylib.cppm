export module raylib;
export import :bridge;
export import :gui;
import std;

export {


  // some utils for formatting for output
  template <typename T>
  concept Point2D = requires(T v) {
    { v.x } -> std::convertible_to<float>;
    { v.y } -> std::convertible_to<float>;
  };

  template <typename T>
  concept Rect2D = Point2D<T> && requires(T v) {
    { v.width } -> std::convertible_to<float>;
    { v.height } -> std::convertible_to<float>;
  };

  template <typename T>
  concept stringable = requires(T a) {
    { to_string(a) };
  };

  constexpr auto to_string(const Point2D auto &v)
    requires(!Rect2D<decltype(v)>)
  {
    return std::format("({:.2f},{:.2f})", v.x, v.y);
  }

  // i dont want to make a formatter for rect atnd point and so on, i prefer
  // to_String to be explicit
  constexpr auto to_string(const Rect2D auto &v) {
    return std::format("(x:{:.2f},y:{:.2f},w:{:.2f},h:{:.2f})", v.x, v.y,
                       v.width, v.height);
  }

  /*
   * get a range and a transformer of those elements into a formattable type
   * returns a string of separated list (separator by default is ;)
   * useful for listviews in raygui
   */
  template <typename TransformerT>
  constexpr auto list_to_datastr(std::string_view name,
                                 std::ranges::range auto data,
                                 TransformerT transformer,
                                 const char separator = ';')

    requires stringable<TransformerT> || std::formattable<TransformerT, char>
  {
    std::string datastr{};
    std::size_t i = 0;
    for (auto &current : data) {
      datastr +=
          std::format("{}{}: {}{}", name, i, transformer(current), separator);
      i++;
    }
    // remove last ';'
    datastr.pop_back();
    return datastr;
  }
}
