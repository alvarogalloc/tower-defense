export module systems.bullet;
import raylib;
import ginseng;
import std;
import components.bullet;

export namespace systems::bullet
{

void shoot_bullet(ginseng::database &db, components::bullet info);

void update(ginseng::database &db, float dt, Rectangle bounds);
void draw(ginseng::database &db);

} // namespace systems::bullet
