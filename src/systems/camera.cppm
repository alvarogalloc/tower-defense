export module systems.camera;
import ginseng;
import components.movement;
import raylib;
import game;

export namespace systems::camera
{
Camera2D make_camera()
{
    Camera2D camera{};
    camera.target = {0.0f, 0.0f};
    camera.offset = {game::get().get_spec().size/2.f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    return camera;
}
void use(Camera2D &camera, ginseng::database &db,
         const ginseng::database::ent_id &target_entity, const auto &render_fn)
{
    BeginMode2D(camera);
    const auto &mov = db.get_component<components::movement>(target_entity);
    camera.target = mov.position;
    render_fn();

    EndMode2D();
}
}; // namespace systems::camera
