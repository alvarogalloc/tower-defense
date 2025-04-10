module prefabs.space_background;
import config;
import std;
import debug;
namespace prefabs {

  space_background::space_background()
      : m_resolution(config::game_info.size)
  {
    using namespace std::string_literals;
    const auto fragment_shader_path = config::game_info.asset_path + "/space_background.fs"s;
    using namespace std::filesystem;
    debug::my_assert(exists(fragment_shader_path), "File does not exist: "s + fragment_shader_path);
    m_shader = LoadShader(nullptr, fragment_shader_path.c_str());
    m_time_loc = GetShaderLocation(m_shader, "time");
    m_res_loc = GetShaderLocation(m_shader, "resolution");
  }

  space_background::~space_background()
  {
    UnloadShader(m_shader);
  }

  void space_background::update(float delta_time)
  {
    m_time += delta_time;
    SetShaderValue(m_shader, m_time_loc, &m_time, SHADER_UNIFORM_FLOAT);
  }

  void space_background::draw()
  {
    // BeginShaderMode(m_shader);
    // DrawRectangle(0, 0, int(m_resolution.x), int(m_resolution.y), colors::black);
    // EndShaderMode();
  }
} // namespace prefabs
