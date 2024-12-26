export module draw_system;
import target_manager;
import std;
import raylib;
import raylib_utils;
import fmt;

#if 0
/*export class draw_system {*/
/*  public:*/
/*  constexpr draw_system(*/
/*    const target_manager& targets, const std::vector<detached_bullet>& detached_bullets,*/
/*    const Vector2& player_pos)*/
/*      : m_target_manager(targets)*/
/*      , m_player_pos(player_pos)*/
/*      , m_detached_bullets(detached_bullets)*/
/*  {}*/
/*  void draw() const*/
/*  {*/
/*    draw_detached_bullets();*/
/*    draw_targets();*/
/*    /*draw_targets_hud();*/
/*  }*/
/**/
/*  private:*/
/*  void draw_detached_bullets() const*/
/*  {*/
/*    for (const auto& b : m_detached_bullets) {*/
/*      if (b.valid()) {*/
/*        DrawCircleV(b.position, b.info->radius, b.info->color);*/
/*        // Draw velocity*/
/*        DrawLineV(b.position, b.position + b.velocity, colors::yellow);*/
/*      }*/
/*    }*/
/*  }*/
/*  void draw_targets() const*/
/*  {*/
/*    constexpr int separation = 20;*/
/*    for (const auto& t : m_target_manager.get_targets()) {*/
/*      DrawText(*/
/*        fmt::format("{}/{}", t.health, t.max_health).c_str(), static_cast<int>(t.pos.x),*/
/*        static_cast<int>(t.pos.y - t.radius) - separation, 20, t.color);*/
/*      auto [closest_x, closest_y] = m_target_manager.closest_to(m_player_pos).pos;*/
/*      if (t.pos.x == closest_x && t.pos.y == closest_y) {*/
/*        DrawCircleV(t.pos, t.radius, colors::white);*/
/*        DrawLineV(t.pos, m_player_pos, colors::white);*/
/*      } else {*/
/*        DrawCircleV(t.pos, t.radius, t.color);*/
/*      }*/
/*    }*/
/*  }*/
/*  void draw_targets_hud() const*/
/*  {*/
/*    constexpr static auto init_pos = Vector2 {20, 20};*/
/*    // size in pixels needed for each row showing the target healthbar and position*/
/*    constexpr static auto item_size = Vector2 {100, 20};*/
/*    constexpr static auto separation {5};*/
/*    constexpr static auto item_font_size = 10;*/
/**/
/*    Vector2 pos = init_pos;*/
/*    for (const auto& target : m_target_manager.get_targets()) {*/
/*      // draw the target*/
/*      // make it a rectangle with green of the remaining health*/
/*      // and red for the lost health*/
/*      const auto health_ratio = target.health / target.max_health;*/
/*      // Red rectangle*/
/*      DrawRectangleV(pos, item_size, colors::red);*/
/*      // Green rectangle*/
/*      const Vector2 adjusted_size {item_size.x * health_ratio, item_size.y};*/
/**/
/*      DrawRectangleV(pos, adjusted_size, colors::green);*/
/**/
/*      const auto str_pos = to_string(target.pos);*/
/*      DrawText(*/
/*        str_pos.c_str(), static_cast<int>(pos.x),*/
/*        static_cast<int>(pos.y + 5 + (item_font_size / 2.f)), item_font_size, colors::white);*/
/**/
/*      pos.y += item_size.y + separation;*/
/*    }*/
/*  }*/
/**/
/*  private:*/
/*  const target_manager& m_target_manager;*/
/*  const Vector2& m_player_pos;*/
/*  const std::vector<detached_bullet>& m_detached_bullets;*/
/*};*/
#endif
