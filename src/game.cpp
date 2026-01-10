module;
#include <cstdio>
module game;
import debug;
namespace {

void custom_raylib_log(int msgType, const char *text, va_list args) {
  std::cout << debug::colors::bold;
  bool should_quit = false;
  switch (msgType) {
    case LOG_INFO:
      std::cout << debug::info << "[INFO]";
      break;
    case LOG_ERROR:
      std::cout << debug::error << "[ERROR]";
      break;
    case LOG_WARNING:
      std::cout << debug::warn << "[WARN]";
      break;
    case LOG_DEBUG:
      std::cout << debug::success << "[DEBUG]";
      break;
  }
  std::cout << ": ";
  std::vprintf(text, args);
  std::cout << debug::reset << '\n';
  debug::my_assert(!should_quit, "raylib error, exiting...");
}
}  // namespace

// new implementation

game_context::game_context(std::string_view game_dir_env_var) : assets(game_dir_env_var) {
  app_info = assets.get_config<config::app_info>("assets/game_info.json");
}
void game::init() {
  SetTraceLogCallback(custom_raylib_log);
  const auto [x, y] = get_app_info().game_res;
  const auto scale = get_app_info().scale;
  const auto wx = x * scale;
  const auto wy = y * scale;
  // unsigned int flags =
  //     FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT;
  std::println("window is {} {} ", wx, wy);
  // SetConfigFlags(flags);
  InitWindow(int(wx), int(wy), get_app_info().window_name.data());
  SetTargetFPS(get_app_info().fps);
  InitAudioDevice();
  SetMouseScale(x / wx, y / wy);
}
void game::run(std::unique_ptr<state> first_state) {
  debug::my_assert(bool(first_state),
                   "cannot start game without a first state");

  m_state.unsafe_change(std::move(first_state));
  m_state.get_current()->on_start();
  bool is_running = true;

  // load target texture
  const auto [_, resolution, scale, _2] = this->get_app_info();
  RenderTexture2D draw_target = LoadRenderTexture(
      static_cast<int>(resolution.x), static_cast<int>(resolution.y));
  SetTextureFilter(draw_target.texture, TEXTURE_FILTER_POINT);

  while (is_running) {
    this->m_state.update();

    BeginTextureMode(draw_target);
    this->m_state.render();
    EndTextureMode();
    BeginDrawing();
    {
      ClearBackground(rooster::colors::black);
      DrawTexturePro(draw_target.texture,
                     Rectangle{
                         0,
                         0,
                         resolution.x,
                         -resolution.y,
                     },
                     Rectangle{
                         0,
                         0,
                         resolution.x * scale,
                         resolution.y * scale,
                     },
                     Vector2{0, 0}, 0.0f, rooster::colors::white);
      EndDrawing();
    }
  }

  std::println("exiting game!!");
}
