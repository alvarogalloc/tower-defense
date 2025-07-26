export module target_manager;
import raylib;
import std;
import debug;
import json;

export {
    struct target
    {
        float radius;
        float health;
        float max_health;

        Vector2 pos;
        Color color;
        bool operator==(const target &rhs) const
        {
            return pos.x == rhs.pos.x && pos.y == rhs.pos.y &&
                   radius == rhs.radius && color.r == rhs.color.r &&
                   color.g == rhs.color.g && color.b == rhs.color.b &&
                   health == rhs.health && max_health == rhs.max_health;
        }
    };

    class target_manager
    {
        std::vector<target> m_targets;

      public:
        target_manager() = default;
        target_manager(std::ifstream &file)
        {
            load_targets_from_file(file);
        }
        target_manager(const std::string &contents)
        {
            load_from_json(contents);
        }
        void load_targets_from_file(std::ifstream &file)
        {
            debug::my_assert(file.is_open(), "file is not open");
            std::string contents =
                std::string(std::istreambuf_iterator<char>(file),
                            std::istreambuf_iterator<char>());
            load_from_json(contents);
        }
        [[nodiscard]] auto &closest_to(const Vector2 pos) const
        {
            return *std::ranges::min_element(
                m_targets,
                [&](const auto &lhs, const auto &rhs) {
                    return std::hypot(lhs.pos.x - pos.x, lhs.pos.y - pos.y) <
                           std::hypot(rhs.pos.x - pos.x, rhs.pos.y - pos.y);
                });
        }

        auto &get_targets()
        {
            return m_targets;
        }
        [[nodiscard]] auto &get_targets() const
        {
            return m_targets;
        }

        void load_from_json(const std::string &contents)
        {
#if 0
            // with nlohmann
            using json = nlohmann::json;
            auto targets = json::parse(contents);
            for (const auto &target : targets)
            {
                m_targets.emplace_back(
                    target["radius"].get<float>(),
                    target["health"].get<float>(),
                    target["max_health"].get<float>(),
                    Vector2{target["pos"]["x"].get<float>(),
                            target["pos"]["y"].get<float>()},
                    Color{target["color"]["r"].get<std::uint8_t>(),
                          target["color"]["g"].get<std::uint8_t>(),
                          target["color"]["b"].get<std::uint8_t>(),
                          target["color"]["a"].get<std::uint8_t>()});
            }
#endif
#if 0
            // with glaze json

            using glz::read_json;
            auto ec = read_json(m_targets, contents);
            debug::my_assert(!ec, "error reading json: {}",
                             ec.custom_error_message);

#endif
#if 1
            using namespace rapidjson;
            static Document doc;
            doc.Parse(contents.c_str());
            debug::my_assert(!doc.HasParseError(), "error parsing json");
            for (const auto &target : doc.GetArray())
            {
                using u8 = std::uint8_t;
                m_targets.emplace_back(
                    target["radius"].GetFloat(), target["health"].GetFloat(),
                    target["max_health"].GetFloat(),
                    Vector2{target["pos"]["x"].GetFloat(),
                            target["pos"]["y"].GetFloat()},
                    Color{u8(target["color"]["r"].GetUint()),
                          u8(target["color"]["g"].GetUint()),
                          u8(target["color"]["b"].GetUint()),
                          u8(target["color"]["a"].GetUint())});
            }

#endif
        }

        target &spawn(target t)
        {
            return m_targets.emplace_back(t);
        }
    };
}
