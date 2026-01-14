module scenes;
import wey;
import raylib;

namespace scenes
{

void gameover::on_start()
{
}
void gameover::on_update()
{
}


struct gameover_json_data {
  Color bg_color;
  Color btn_color;
  Color fg_color;
  int font_size;
};
void gameover::on_render() 
{
    const std::string title =
        std::format("Game Over - \n{}", stats.won ? "You Win!" : "You Lose!");

    const std::string score = std::format("Score: {}\nTime: {} seconds",
                                          stats.score, stats.time_seconds);
    const auto [bg_color, btn_color, txt_color, font_size] = ctx.assets.get_config<gameover_json_data>("gameover.json");

    ClearBackground(bg_color);
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(btn_color));
    GuiSetStyle(DEFAULT, TEXT_SIZE, font_size);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(txt_color));
    auto [width, height] = ctx.app_info.game_res;
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
