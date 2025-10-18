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
    int max;
    int current;
};

struct gun
{
    float reload_time;
    float last_shot;
    int ammo;
    int max_ammo;
    std::string shoot_sfx_path;
};
void update(ginseng::database &db, float dt);
void draw(ginseng::database &db);

} // namespace systems::player
