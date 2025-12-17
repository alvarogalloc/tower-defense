module;
#include <cstdio>
module game;
import debug;
namespace {

void custom_raylib_log(int msgType, const char *text, va_list args) {
  return;
  std::cout << debug::colors::bold;
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
}
// NOLINTNEXTLINE
static game *g_instance = nullptr;
}  // namespace

game::game() {
  SetTraceLogCallback(custom_raylib_log);
  const auto [x, y] = get_config().get_app_info().game_res;
  auto wx = x * m_cfg.get_app_info().scale;
  auto wy = y * get_config().get_app_info().scale;
  unsigned int flags =
      FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT;
  std::println("window is {} {} ", wx, wy);
  SetConfigFlags(flags);
  InitWindow(int(wx), int(wy), get_config().get_app_info().window_name.data());
  SetTargetFPS(get_config().get_app_info().fps);
  InitAudioDevice();
  m_target = LoadRenderTexture(int(x), int(y));
  SetTextureFilter(m_target.texture, TEXTURE_FILTER_POINT);
  SetMouseScale(x / wx, y / wy);
  g_instance = this;
}
game &game::get() { return *g_instance; }
void game::exit() {
  auto _ = m_scene->on_exit();
  if (m_scene) {
    this->m_scene.reset();
  }
  CloseAudioDevice();
  CloseWindow();
}
void game::set_scene(std::unique_ptr<scene> scene) {
  this->m_scene.reset();
  this->m_scene = std::move(scene);
  this->m_scene->on_start();
}
void game::draw_debug_messages() const {
  if (m_debug_messages.empty()) {
    return;
  }
  // makea grayish rectangle in the right thirsd of the screen
  // const auto col = Color{50, 50, 50, 200};
  auto [x, y] = get_config().get_app_info().game_res;
  const auto rect = Rectangle{x * 2 / 3.f, 0, x / 3.0f, y};

  // use GuiListView
  // make a string from the debug messages joined by semicolon
  auto result = std::accumulate(
      m_debug_messages.begin(), m_debug_messages.end(), std::string{},
      [](std::string a, const debug::message &b) {
        if (b.lifetime_seconds <= 0 and b.lifetime_seconds != -1) {
          return a;
        }
        return std::move(a) +
               std::format("{} ({:.2});", b.text, b.lifetime_seconds);
      });
  result.pop_back();  // remove last semicolon
  static int scroll_index = 0;
  int active = -1;
  GuiListView(rect, result.c_str(), &scroll_index, &active);
}

int game::run() {
  try {
    bool debug_mode = false;
    debug::my_assert(bool(m_scene), "Scene not set");
    while (!WindowShouldClose()) {
      if (m_scene->should_exit_game()) {
        break;
      }
      if (m_scene->should_exit()) {
        auto new_scene = m_scene->on_exit();
        if (!new_scene) {
          break;
        }
        this->set_scene(std::move(new_scene));
      }
      m_scene->on_update();
      // f1 to toggle debug
      debug_mode = IsKeyPressed(KEY_TWO) ? !debug_mode : debug_mode;
      std::ranges::for_each(m_debug_messages, [dt = GetFrameTime()](auto &msg) {
        if (msg.lifetime_seconds > 0) msg.lifetime_seconds -= dt;
      });

      BeginTextureMode(m_target);
      {
        m_scene->on_render();
        if (debug_mode) {
          this->draw_debug_messages();
        }
        EndTextureMode();
      }
      BeginDrawing();
      {
        ClearBackground(rooster::colors::black);
        DrawTexturePro(m_target.texture,
                       Rectangle{
                           0,
                           0,
                           get_config().get_app_info().game_res.x,
                           -get_config().get_app_info().game_res.y,
                       },
                       Rectangle{
                           0,
                           0,
                           get_config().get_app_info().game_res.x *
                               get_config().get_app_info().scale,
                           get_config().get_app_info().game_res.y *
                               get_config().get_app_info().scale,
                       },
                       Vector2{0, 0}, 0.0f, rooster::colors::white);
        EndDrawing();
      }
      // {
      //   ClearBackground(rooster::colors::blue);
      //   DrawTexturePro(m_target.texture,
      //                  Rectangle{
      //                      0,
      //                      0,
      //                      m_spec.game_res.x,
      //                      -m_spec.game_res.y,
      //                  },
      //                  Rectangle{
      //                      0,
      //                      0,
      //                      m_spec.size.x,
      //                      m_spec.size.y,
      //                  },
      //                  Vector2{0, 0}, 0.0f, rooster::colors::white);
      //   EndDrawing();
      // }
    }
    this->exit();
    return 0;
  } catch (const std::exception &e) {
    std::println("{}Exception Caught: {}{}", debug::error, e.what(),
                 debug::reset);
    this->exit();
    return -1;
  } catch (...) {
    std::print("Error: Unknown error\n");
    this->exit();
    return -1;
  }
}
