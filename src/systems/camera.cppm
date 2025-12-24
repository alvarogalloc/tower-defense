export module systems.camera;
import components.movement;
import raylib;
import game;
import entt;

export namespace systems::camera {
Camera2D make_camera(const Vector2 game_res) {
  const auto init_pos = game_res * 0.5f;
  Camera2D camera{
      .offset = init_pos,
      .target{0.f, 0.f},
      .rotation = 0.0f,
      .zoom = 1.0f,
  };
  return camera;
}
void use(Camera2D &camera, entt::registry &db,
         const entt::entity target_entity,
         const auto &render_fn) {
  const auto &mov = db.get<components::movement>(target_entity);
  camera.target = mov.position;
  BeginMode2D(camera);
  render_fn();

  EndMode2D();
}
};  // namespace systems::camera
