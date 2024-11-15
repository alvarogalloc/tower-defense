export module debug_gui_system;
import bullets;
import target_manager;
import std;
import raylib;
import fmt;
import raygui;
import debug;
import raylib_utils;

export
{
  class debug_gui_system {
private:
    const std::vector<bullets>& m_bullet_rings;
    const target_manager& m_t_manager;

    int scrollIndex = 0, active = 0, ring_list_scroll = 0, ring_list_active = 0,
        target_list_scroll = 0, target_list_active = 0;

public:
    debug_gui_system(const std::vector<bullets>& bullet_rings, const target_manager& t_manager):
      m_bullet_rings(bullet_rings), m_t_manager(t_manager)
    {}
    void draw(const std::size_t current_ring)
    {
      constexpr static auto list = "Bullet Rings;Current Ring;Targets;Closest Target";
      constexpr static Rectangle first_quadrant {0, 0, 200, 200};
      constexpr static Rectangle second_quadrant {200, 0, 200, 200};
      constexpr static Rectangle third_quadrant {400, 0, 200, 200};
      constexpr static Color background_color = colors::raywhite;
      constexpr static Rectangle label_rect {410, 0, 190, 200};

      GuiListView(first_quadrant, list, &scrollIndex, &active);
      auto draw_ring_info = [](const auto& ring) {
        auto [damage, radius, speed, color, count, ellipse_radius] = ring.get_info();
        // draw rectangle with color
        std::string info = fmt::format(
          "Damage: {}\nRadius: {}\nSpeed: {}\nColor: {:x}\nCount: {}\nRadius: {}", damage, radius,
          speed, static_cast<std::uint32_t>(ColorToInt(color)), count, to_string(ellipse_radius));
        DrawRectangleRec(third_quadrant, background_color);

        GuiLabel(label_rect, info.c_str());
      };
      auto draw_target_info = [](const auto& target) {
        auto [pos, radius, color, health, max_health] = target;
        std::string info = fmt::format(
          "Position: {}\nRadius: {}\nColor: {:x}\nHealth: {}\nMax Health: {}", to_string(pos),
          radius, static_cast<std::uint32_t>(ColorToInt(color)), health, max_health);
        DrawRectangleRec(third_quadrant, background_color);
        GuiLabel(label_rect, info.c_str());
      };
      switch (active) {
        case 0: {
          // show in second quadrant, the list of bullet rings and show the info of the selected
          if (m_bullet_rings.empty()) {
            break;
          }
          auto list_str = list_to_datastr("", m_bullet_rings, [&](const auto& ring) {
            return fmt::format(
              "Ring {:x}", static_cast<std::uint32_t>(ColorToInt(ring.get_info().color)));
          });
          GuiListView(second_quadrant, list_str.c_str(), &ring_list_scroll, &ring_list_active);
          auto index = static_cast<std::size_t>(ring_list_active);
          debug::my_assert(index >= 0 && index < m_bullet_rings.size(), "Invalid ring index");
          // cast to std::size_t to avoid warning of narrowing conversion from int to std::size_t
          draw_ring_info(m_bullet_rings.at(index));
          break;
        }
        case 1: {
          if (current_ring >= m_bullet_rings.size()) {
            break;
          }
          draw_ring_info(m_bullet_rings.at(current_ring));
          break;
        }
        case 2: {
          if (m_t_manager.get_targets().empty()) {}
          auto list_str = list_to_datastr("", m_t_manager.get_targets(), [&](const auto& target) {
            return fmt::format("Target {:x}", static_cast<std::uint32_t>(ColorToInt(target.color)));
          });
          GuiListView(second_quadrant, list_str.c_str(), &target_list_scroll, &target_list_active);
          draw_target_info(m_t_manager.get_targets().at(std::size_t(target_list_active)));
          break;
        }
        case 3: {
          if (m_t_manager.get_targets().empty()) {
            break;
          }
          draw_target_info(m_t_manager.closest_to(GetMousePosition()));
          break;
        }
        default: break;
      }
    }
  };

} // namespace systems