export module shooting_system;
import std;
import raylib;
struct bullet {
    Vector2 position;
    Vector2 velocity;
    float deceleration_rate;
    bool is_valid;
};
// makes new bullets out of rings
/*export std::function<std::optional<detached_bullet>()>*/
/*make_shooting_system(std::vector<bullets>& bullet_rings)*/
/*{*/
/*  return [&, selected_ring = 0uz, aim_vector = Vector2 {1.f, 0.f}]() mutable {*/
/*    if (IsKeyPressed(KEY_TAB)) {*/
/*      selected_ring = (selected_ring + 1) % bullet_rings.size();*/
/*    }*/
/*    // key left and right to angle*/
/*    if (IsKeyDown(KEY_LEFT)) {*/
/*      aim_vector = Vector2Rotate(aim_vector, -0.1f);*/
/*    }*/
/*    if (IsKeyDown(KEY_RIGHT)) {*/
/*      aim_vector = Vector2Rotate(aim_vector, 0.1f);*/
/*    }*/
/*    return bullet_rings.at(selected_ring).detach_bullet(aim_vector);*/
/*  };*/
/*}*/
/**/
/*export std::function<void(float)> make_update_bullets(std::vector<detached_bullet>& detached)*/
/*{*/
/*  return [&](float dt) {*/
/*    for (auto& b : detached) {*/
/*      if (b.valid()) {*/
/*        Vector2 deceleration*/
/*          = Vector2Scale(Vector2Normalize(b.velocity), -b.deceleration_rate * dt);*/
/*        b.velocity = Vector2Add(b.velocity, deceleration);*/
/**/
/*        // Stop if velocity is very low*/
/*        if (Vector2Length(b.velocity) < 10.0f) {*/
/*          b.velocity = {0.0f, 0.0f};*/
/*        }*/
/**/
/*        // Update position*/
/*        b.position = Vector2Add(b.position, Vector2Scale(b.velocity, dt));*/
/*      }*/
/*    }*/
/*  };*/
/*}*/
/**/
/*export void draw_detached_bullets(std::span<detached_bullet>& detached)*/
/*{*/
/*  for (const auto& drawable : detached) {*/
/*    (void) drawable;*/
/*  }*/
/*}*/
