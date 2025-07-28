module scenes.menu;
import raygui;
import scenes.shooting;
import asset_routes;
import gui;
using namespace rooster;
inline constexpr int font_size = 96;
inline constexpr Color base_color{.r = colors::darkpurple.r,
                                  .g = colors::darkpurple.g,
                                  .b = colors::darkpurple.b,
                                  .a = 100};

namespace scenes
{

menu::menu()
    : m_space_background(SRC_DIR "/assets/title_bg/blue-back.png",
                         std::vector<std::string>{
                             SRC_DIR "/assets/title_bg/prop-planet-big.png",
                             SRC_DIR "/assets/title_bg/prop-planet-small.png"},
                         0.5f)
{
}
void menu::on_start()
{
    m_title_font = LoadFont(routes::TITLE_FONT);
    GuiSetFont(m_title_font);
    m_blue_guy = LoadTexture(routes::BLUE_GUY_TEXTURE);
    m_music = LoadMusicStream(routes::MENU_MUSIC);
    PlayMusicStream(m_music);

    auto base_color = colors::darkpurple;
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(base_color));

    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, ColorToInt(colors::white));
    GuiSetStyle(0, TEXT_SIZE, font_size);
}

void menu::on_update()
{

    m_space_background.update();
    UpdateMusicStream(m_music);
}

std::unique_ptr<scene> menu::on_exit()
{
    UnloadTexture(m_blue_guy);
    // UnloadFont(m_title_font); font is shared with raygui so we don't unload
    // it
    return std::make_unique<scenes::shooting>();
}

void menu::on_render()
{
    ClearBackground(colors::darkblue);
    m_space_background.draw();

    float next_element_y = 0;

    [&]() {
        constexpr static std::string_view title = "Rooster\nSpace";
        const auto text_size =
            MeasureTextEx(m_title_font, title.data(), 120, 0.2f);
        const auto text_pos = Vector2{
            .x = float(GetScreenWidth()) / 2 - text_size.x / 2, .y = 100};
        // draw a brown rectangle behind the text
        const auto title_rect = Rectangle{text_pos.x - 10, text_pos.y,

                                          text_size.x + 10, text_size.y + 10};
        const float padding_y = 10.f;
        const float text_space = 0.2f;
        DrawRectangleRec(title_rect, base_color);
        DrawTextEx(m_title_font, title.data(), text_pos, font_size, text_space,
                   colors::yellow);
        next_element_y = padding_y + title_rect.y + title_rect.height;
    }(); // draw title

    using namespace gui::v2;
    const auto window_size = game::get().get_spec().size;

    container menu_container{
        .size = {window_size.x / 4, window_size.y / 3},
    };
    menu_container.gap = 25.f;
    const Vector2 btn_size{menu_container.size.x-50.f, 75.f};
    menu_container.position =
        (window_size / 2) - (menu_container.size / 2) + Vector2{0, 200};
    menu_container.add_child<button>("START", colors::yellow, btn_size,
                                     [&](button &) {
                                         m_should_exit = true;
                                         std::puts("Starting game!!\n");
                                     });
    menu_container.add_child<button>(
        "EXIT", colors::yellow, btn_size,
        [&](button &) { m_should_exit_game = true; });

    menu_container.draw();
}
} // namespace scenes
