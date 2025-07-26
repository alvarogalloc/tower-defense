export module systems.player;
import raylib;
import std;
import ginseng;
import components.movement;

export namespace systems::player
{

enum class action : std::uint8_t
{
    none,
    shoot,
    reload,
};
struct health
{
    std::uint8_t max;
    std::uint8_t current;
    health(std::uint8_t mx) : max(mx), current(mx)
    {
    }
};

struct gun
{
    float reload_time;
    float last_shot;
    std::uint8_t ammo;
    std::uint8_t max_ammo;
    Sound shoot_sfx;
};
void update(ginseng::database &db, float dt);
void draw(ginseng::database &db);

} // namespace systems::player
