
module scenes.gameover;
import config;
import raylib;
import raygui;

namespace scenes
{

gameover::gameover(game_stats stats) : stats(stats)
{
}
void gameover::on_start()
{
}
void gameover::on_update()
{
}

void gameover::on_render()
{
    const std::string title =
        std::format("Game Over - \n{}", stats.won ? "You Win!" : "You Lose!");

    const std::string score = std::format("Score: {}\nTime: {} seconds",
                                          stats.score, stats.time_seconds);
    const auto &game_cfg = config::get_game_config();
    // pallete of bg and text
    const auto color_from_json = [](const auto &v) -> Color {
        return Color{static_cast<unsigned char>(v[0].GetInt()),
                     static_cast<unsigned char>(v[1].GetInt()),
                     static_cast<unsigned char>(v[2].GetInt()),
                     static_cast<unsigned char>(v[3].GetInt())};
    };
    const auto bg_color = color_from_json(game_cfg["gameover"]["bg_color"]);
    const auto btn_color = color_from_json(game_cfg["gameover"]["btn_color"]);
    const auto txt_color = color_from_json(game_cfg["gameover"]["fg_color"]);
    const auto font_size = game_cfg["gameover"]["font_size"].GetInt();

    ClearBackground(bg_color);
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(btn_color));
    GuiSetStyle(DEFAULT, TEXT_SIZE, font_size);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(txt_color));
    auto [width, height] = game::get().get_spec().size;
    const auto label_rect = Vector2{0, 0};
    DrawTextEx(GetFontDefault(), title.c_str(), {label_rect.x, label_rect.y},
               float(font_size), 2.0f, txt_color);

    const auto score_rect = Vector2{0, 250};
    DrawTextEx(GetFontDefault(), score.c_str(), {score_rect.x, score_rect.y},
               float(font_size), 2.0f, txt_color);

    const auto button_rect = Rectangle{0, height - 250, 400, 200};
    if (GuiButton(button_rect, "Exit"))
    {
        m_should_exit = true;
    }
}
} // namespace scenes
