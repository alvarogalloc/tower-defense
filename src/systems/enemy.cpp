module systems.enemy;
import components.enemy;
import systems.player;
import components.bullet;
import components.movement;
import components.tags;
import debug;
import std;
import raylib;
using namespace rooster;
namespace systems::enemy
{
std::function<ginseng::database::ent_id(ginseng::database &, float, float)>
make_spawner(const components::bounding_box box, const components::enemy enemy)
{
    return [box, enemy](ginseng::database &db, float x, float y) {
        // this call represent a new enemy with the 'enemy' preset
        auto id = db.create_entity();
        const auto new_box =
            components::bounding_box{x, y, box.width, box.height};
        db.add_component(id, new_box);
        db.add_component(id, enemy);
        return id;
    };
}

void update(ginseng::database &db, float dt)
{

    auto player_id = [&db] {
        std::optional<ginseng::database::ent_id> player_id;
        db.visit([&player_id](ginseng::database::ent_id id,
                              components::tags::player) { player_id = id; });
        debug::my_assert(bool(player_id), "player not found");
        return *player_id;
    }();

    Vector2 player_pos =
        db.get_component<components::movement>(player_id).position;
    auto &player_health = db.get_component<systems::player::health>(player_id);
    // std::println("player pos: (x: {}, y: {})", player_pos.x, player_pos.y);
    db.visit([dt, player_pos, &player_health](components::enemy &e,
                                              components::bounding_box &b) {
        Vector2 direction = Vector2Subtract(player_pos, {b.x, b.y});
        float distance = Vector2Length(direction);
        if (distance > 0)
        {
            direction = Vector2Scale(Vector2Normalize(direction), e.speed * dt);
        }
        const float damage_threeshold = 5.f;
        if (distance > damage_threeshold)
        {
            static float time_to_next_hit = 0.f;
            const float time_between_hits_s = 3.f;
            const std::uint8_t damage_p_hit = 10;
            time_to_next_hit -= GetFrameTime();
            if (time_to_next_hit <= 0.f && player_health.current >= damage_p_hit)
            {
                std::puts("damaging player\n");
                player_health.current -= damage_p_hit;
                time_to_next_hit = time_between_hits_s;
            }
        }
        auto [x, y] = Vector2Add({b.x, b.y}, direction);
        b.x = x;
        b.y = y;
    });
}
void draw(ginseng::database &db)
{
    db.visit([](components::enemy &e, components::bounding_box &box) {
        const float thickness = 2.f;
        const auto color = colors::red;

        DrawRectangleLinesEx(box, thickness, color);
        DrawTextureEx(e.texture, {box.x, box.y}, 0.f, 1.f, colors::white);
        DrawText(std::format("{}/{}", e.health, e.speed).c_str(), int(box.x),
                 int(box.y) - 20, 20, colors::white);
    });
}

} // namespace systems::enemy
