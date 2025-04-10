export module prefabs.space_background;
import raylib;

export namespace prefabs{
   class space_background {
public:
    space_background();
    ~space_background();

    void update(float delta_time);
    void draw();

private:
    Shader m_shader {};
    int m_time_loc {};
    int m_res_loc {};
    float m_time = 0.0f;
    Vector2 m_resolution {};
  };
} // namespace
