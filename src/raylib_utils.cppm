export module raylib_utils;
import raylib;
import std;

export
{
  constexpr auto to_string(const Vector2& v)
  {
    return std::format("({:.2f},{:.2f})", v.x, v.y);
  }
  constexpr auto operator+(const Vector2& lhs, const Vector2& rhs)
  {
    return Vector2 {lhs.x + rhs.x, lhs.y + rhs.y};
  }
  constexpr auto operator+(const Vector2& lhs, const float rhs)
  {
    return Vector2 {lhs.x + rhs, lhs.y + rhs};
  }

  constexpr auto operator-(const Vector2& lhs, const Vector2& rhs)
  {
    return Vector2 {lhs.x - rhs.x, lhs.y - rhs.y};
  }

  constexpr auto operator*(const Vector2& lhs, float rhs)
  {
    return Vector2 {lhs.x * rhs, lhs.y * rhs};
  }

  constexpr auto operator*(float lhs, const Vector2& rhs)
  {
    return Vector2 {lhs * rhs.x, lhs * rhs.y};
  }

  constexpr auto operator*(const Vector2& lhs, const Vector2& rhs)
  {
    return Vector2 {lhs.x * rhs.x, lhs.y * rhs.y};
  }

  constexpr auto list_to_datastr(std::string name, std::ranges::range auto data, auto transformer)
  {
    std::string datastr;
    std::size_t i = 0;
    for (auto& current : data) {
      datastr += std::format("{}{}: {};", name, i, transformer(current));
      i++;
    }
    datastr.pop_back(); // remove last ';'
    return datastr;
  }
}
