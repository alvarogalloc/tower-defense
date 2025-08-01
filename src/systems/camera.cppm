export module systems.camera;
import ginseng;
import components.movement;
import raylib;
import game;

export namespace systems::camera
{
Camera2D make_camera()
{
    const auto init_pos = game::get().get_spec().game_res * 0.5f;
    Camera2D camera{
        .offset = init_pos,
        .target{0.f, 0.f},
        .rotation = 0.0f,
        .zoom = 1.0f,
    };
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
