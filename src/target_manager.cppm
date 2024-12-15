export module target_manager;
import raylib;
import std;
import debug;
import fmt;
import rapidjson;

export
{
  struct target {
    bool operator==(const target& rhs) const
    {
      return pos.x == rhs.pos.x && pos.y == rhs.pos.y && radius == rhs.radius
        && color.r == rhs.color.r && color.g == rhs.color.g && color.b == rhs.color.b
        && health == rhs.health && max_health == rhs.max_health;
    }
    Vector2 pos;
    float radius;
    Color color;
    float health;
    float max_health;
  };

  class target_manager {
    std::vector<target> m_targets;

public:
    constexpr target_manager() = default;
    target_manager(std::ifstream&& file)
    {
      load_targets_from_file(std::move(file));
    }
    constexpr target_manager(const std::string& contents)
    {
      load_from_json(contents);
    }
    void load_targets_from_file(std::ifstream&& file)
    {
      debug::my_assert(file.is_open(), "file is not open");
      std::string contents
        = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
      load_from_json(contents);
    }
    constexpr auto& closest_to(const Vector2 pos) const
    {
      return *std::min_element(
        m_targets.begin(), m_targets.end(), [&](const auto& lhs, const auto& rhs) {
          return std::hypot(lhs.pos.x - pos.x, lhs.pos.y - pos.y)
            < std::hypot(rhs.pos.x - pos.x, rhs.pos.y - pos.y);
        });
    }

    constexpr auto& get_targets()
    {
      return m_targets;
    }
    constexpr auto& get_targets() const
    {
      return m_targets;
    }

    constexpr void load_from_json(const std::string& contents)
    {
      rapidjson::Document doc;
      doc.Parse(contents.c_str());
      // shold be an array of objects
      debug::my_assert(doc.IsArray(), "The json should contain an array of objects");

      for (const auto& target : doc.GetArray()) {
        debug::my_assert(target.IsObject(), "Each element of the array should be an object");

        debug::my_assert(target.HasMember("pos"), "Each target should have a pos");
        debug::my_assert(target["pos"].IsArray(), "Each target should have a pos as an array");
        debug::my_assert(
          target["pos"].Size() == 2, "Each target should have a pos with 2 elements (x, y)");

        debug::my_assert(target.HasMember("radius"), "Each target should have a radius");

        debug::my_assert(target.HasMember("color"), "Each target should have a color");
        debug::my_assert(target["color"].IsArray(), "Each target should have a color as an array");
        debug::my_assert(
          target["color"].Size() == 4,
          "Each target should have a color with 4 elements (r, g, b, a)");

        debug::my_assert(target.HasMember("health"), "Each target should have a health");

        debug::my_assert(target.HasMember("max_health"), "Each target should have a max_health");

        m_targets.emplace_back(
          Vector2 {target["pos"][0].GetFloat(), target["pos"][1].GetFloat()},
          target["radius"].GetFloat(),
          Color {
            static_cast<unsigned char>(target["color"][0].GetInt()),
            static_cast<unsigned char>(target["color"][1].GetInt()),
            static_cast<unsigned char>(target["color"][2].GetInt()),
            static_cast<unsigned char>(target["color"][3].GetInt())},
          target["health"].GetFloat(), target["max_health"].GetFloat());
      }
    }

    constexpr target& spawn(target t)
    {
      return m_targets.emplace_back(t);
    }
  };
}
