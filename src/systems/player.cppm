export module systems.player;
import raylib;
import std;
import ginseng;
import components.movement;
import system_manager;
import event_system;

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

// New unified system function with phase support
void system(ginseng::database &db, float dt, systems::Phase phase, systems::EventBus* event_bus = nullptr);

// Legacy functions for backward compatibility
void update(ginseng::database &db, float dt);
void draw(ginseng::database &db);

} // namespace systems::player
