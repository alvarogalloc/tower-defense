export module prefabs.space_background;
import raylib;
import std;
import state;

export namespace prefabs {

struct json_data {
  std::string bg_texture_path;
  std::vector<std::string> objects_textures_paths;
  float scroll_speed{};
};
class space_background {
 public:
  space_background(const json_data data, context_view ctx);
  space_background(std::string_view json_data_path, context_view ctx)
      : space_background(
            ctx.assets.get_config<json_data>(json_data_path), ctx) {}

  void update();
  void draw() const;

 private:
  Texture2D m_background;
  std::vector<std::tuple<Texture2D, Vector2, float>> m_object_data;
  float m_scroll_speed;
  context_view m_context;
};

}  // namespace prefabs
