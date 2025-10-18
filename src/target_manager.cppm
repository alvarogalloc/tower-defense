export module target_manager;
import raylib;
import std;
import debug;
import glaze;

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
                m_targets, [&](const auto &lhs, const auto &rhs) {
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
            // with glaze json

            using glz::read_json;
            auto ec = read_json(m_targets, contents);
            debug::my_assert(!ec, std::format("error reading json: {}",
                                              glz::format_error(ec, contents)));
        }

        target &spawn(target t)
        {
            return m_targets.emplace_back(t);
        }
    };
}
