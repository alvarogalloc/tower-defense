module;
#include <cstdio>
module game;
import debug;
namespace
{

void custom_raylib_log(int msgType, const char *text, va_list args)
{
    std::cout << debug::colors::bold;
    switch (msgType)
    {

    case LOG_INFO:
        std::cout << "\033[38;5;27m" << "[INFO]";
        break;
    case LOG_ERROR:
        std::cout << "\033[38;5;9m" << "[ERROR]";
        break;
    case LOG_WARNING:
        std::cout << "\033[38;5;208m" << "[WARN]";
        break;
    case LOG_DEBUG:
        std::cout << "\033[38;5;214m" << "[DEBUG]";
        break;
    }
    std::cout << ": ";
    std::vprintf(text, args);
    std::cout << debug::reset << '\n';
}
// NOLINTNEXTLINE
game *g_instance = nullptr;
} // namespace

game::game(config::app_info spec) : m_spec(std::move(spec))
{
    SetTraceLogCallback(custom_raylib_log);
    InitWindow(static_cast<int>(spec.size.x), static_cast<int>(spec.size.y),
               spec.window_name.data());
    SetTargetFPS(spec.fps);
    InitAudioDevice();
    g_instance = this;
}
game &game::get()
{
    return *g_instance;
}
void game::exit()
{
    auto _ = m_scene->on_exit();
    if (m_scene)
    {
        this->m_scene.reset();
    }
    CloseAudioDevice();
    CloseWindow();
}
void game::set_scene(std::unique_ptr<scene> scene)
{
    this->m_scene.reset();
    this->m_scene = std::move(scene);
    this->m_scene->on_start();
}
int game::run()
{
    try
    {
        bool debug_mode = false;
        debug::my_assert(bool(m_scene), "Scene not set");
        while (!WindowShouldClose())
        {
            if (m_scene->should_exit_game())
            {
                break;
            }
            if (m_scene->should_exit())
            {
                auto new_scene = m_scene->on_exit();
                if (!new_scene)
                {
                    break;
                }
                this->set_scene(std::move(new_scene));
            }
            m_scene->on_update();
            // f1 to toggle debug
            debug_mode = IsKeyPressed(KEY_F1) ? !debug_mode : debug_mode;

            if (debug_mode)
            {
                debug::draw_debug(m_debug_messages);
            }
            BeginDrawing();
            {
                m_scene->on_render();
            }

            EndDrawing();
        }
        this->exit();
        return 0;
    }
    catch (const std::exception &e)
    {
        std::print("{}Error: {}\n{}", debug::error, e.what(), debug::reset);
        this->exit();
        return -1;
    }
    catch (...)
    {
        std::print("{}Error: Unknown error\n{}", debug::error, debug::reset);
        this->exit();
        return -1;
    }
}
