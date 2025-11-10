export module systems.bullet;
import raylib;
import ginseng;
import std;
import components.bullet;
import system_manager;
import event_system;

export namespace systems::bullet
{

void shoot_bullet(ginseng::database &db, components::bullet info);

// New unified system function with phase support
void system(ginseng::database &db, float dt, systems::Phase phase, Rectangle bounds, systems::EventBus* event_bus = nullptr);

// Legacy functions for backward compatibility
void update(ginseng::database &db, float dt, Rectangle bounds);
void draw(ginseng::database &db);

} // namespace systems::bullet
