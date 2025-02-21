module scenes.menu;
import raygui;
import scenes.shooting;
import asset_routes;

namespace scenes {

  void menu::on_start()
  {
    m_title_font = LoadFont(routes::TITLE_FONT);
    GuiSetFont(m_title_font);
    m_blue_guy = LoadTexture(routes::BLUE_GUY_TEXTURE);
  }

  void menu::on_update() {}

  std::unique_ptr<scene> menu::on_exit()
  {
    UnloadTexture(m_blue_guy);
    // UnloadFont(m_title_font); font is shared with raygui so we don't unload it
    return std::make_unique<scenes::shooting>();
  }

  void menu::on_render()
  {
    // DrawTexture(*m_background, 0, 0, colors::white);
    ClearBackground(colors::darkgreen);
    // calculate the width of the text
    constexpr static std::string_view title = "Rooster\nSpace";
    const auto text_size = MeasureTextEx(m_title_font, title.data(), 120, 0.2f);
    const auto text_pos = Vector2 {.x = float(GetScreenWidth()) / 2 - text_size.x / 2, .y = 100};
    const int font_size = 96;
    // draw a brown rectangle behind the text
    auto title_rect = Rectangle {text_pos.x - 10, text_pos.y, text_size.x + 10, text_size.y + 10};
    DrawRectangleRec(title_rect, colors::brown);
    DrawTextEx(m_title_font, title.data(), text_pos, font_size, 0.2f, colors::yellow);

    DrawTextureEx(
      m_blue_guy,
      Vector2 {
        float(GetScreenWidth()) / 2.f - float(m_blue_guy.width) / 2.f,
        float(GetScreenHeight() - m_blue_guy.height * 3)},
      0, 3, colors::white);
    GuiSetStyle(DEFAULT, TEXT_SIZE, font_size);
    Rectangle start_rect = title_rect;
    start_rect.y += title_rect.height + 10;
    start_rect.x = float(GetScreenWidth()) / 2 - 125;
    start_rect.width = 250;
    start_rect.height = 80;
    if (GuiButton(start_rect, "START")) {
      m_should_exit = true;
    }
    start_rect.y += start_rect.height + 10;
    if (GuiButton(start_rect, "EXIT")) {
      m_should_exit_game = true;
    }
  }
} // namespace scenes
