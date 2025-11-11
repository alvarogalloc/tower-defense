export module systems.bullet;
import raylib;
import entt;
import std;
import components.bullet;

export namespace systems::bullet
{

void shoot_bullet(entt::registry &db, components::bullet info);

void update(entt::registry &db, float dt, Rectangle bounds);
void draw(entt::registry &db);

} // namespace systems::bullet
