export module prefabs.space_background;
import raylib;
import std;

import config;
export namespace prefabs
{
class space_background
{
  public:
    space_background();
    ~space_background();

    void update(float delta_time);
    void draw();

  private:
    Shader m_shader{};
    int m_time_loc{};
    int m_res_loc{};
    float m_time = 0.0f;
    Vector2 m_resolution{};
};
namespace v2
{

class space_background
{
  public:
    space_background(std::string_view background_texture_path,
                     std::vector<std::string> const &object_texture_paths,
                     const float scroll_speed);
    space_background(config::space_bg const &);
    ~space_background();

    void update();
    void draw() const;

  private:
    Texture2D m_background;
    std::vector<std::tuple<Texture2D, Vector2, float>> m_object_data;

    float m_scroll_speed;
};

} // namespace v2
} // namespace prefabs
