export module raylib_utils;
import raylib;
import std;

export {
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
  constexpr auto to_string(const Point2D auto &v)
    requires(!Rect2D<decltype(v)>)
  {
    return std::format("({:.2f},{:.2f})", v.x, v.y);
  }

  constexpr auto to_string(const Rect2D auto &v) {
    return std::format("(x:{:.2f},y:{:.2f},w:{:.2f},h:{:.2f})", v.x, v.y, v.width,
                       v.height);
  }

  constexpr auto list_to_datastr(std::string_view name,
                                 std::ranges::range auto data,
                                 auto transformer) {
    std::string datastr;
    std::size_t i = 0;
    for (auto &current : data) {
      datastr += std::format("{}{}: {};", name, i, transformer(current));
      i++;
    }
    datastr.pop_back();  // remove last ';'
    return datastr;
  }
}
