module scenes.menu;
import raygui;
import scenes.shooting;
import asset_routes;
inline constexpr int font_size = 96;
inline constexpr Color base_color {
  .r = colors::darkpurple.r,
  .g = colors::darkpurple.g,
  .b = colors::darkpurple.b,
  .a = 100
};


namespace scenes {

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
    // UnloadFont(m_title_font); font is shared with raygui so we don't unload it
    return std::make_unique<scenes::shooting>(*m_game);
  }

  void menu::on_render()
  {
    ClearBackground(colors::darkblue);
    m_space_background.draw();

    float next_element_y = 0;


    [&]() {
      constexpr static std::string_view title = "Rooster\nSpace";
      const auto text_size = MeasureTextEx(m_title_font, title.data(), 120, 0.2f);
      const auto text_pos = Vector2 {.x = float(GetScreenWidth()) / 2 - text_size.x / 2, .y = 100};
      // draw a brown rectangle behind the text
      auto title_rect = Rectangle {text_pos.x - 10, text_pos.y, text_size.x + 10, text_size.y + 10};
      DrawRectangleRec(title_rect, base_color);
      DrawTextEx(m_title_font, title.data(), text_pos, font_size, 0.2f, colors::yellow);
      next_element_y = title_rect.y + title_rect.height + 10;
    }();

    [&]() {
      DrawTextureEx(
        m_blue_guy,
        Vector2 {
          float(GetScreenWidth()) / 2.f - float(m_blue_guy.width) / 2.f,
          float(GetScreenHeight() - m_blue_guy.height * 3)},
        0, 3, colors::white);
    }();

    enum class action { start, exit, none };
    auto current_action = [&]() {

      Rectangle start_rect;
      start_rect.y += next_element_y + 10;
      start_rect.x = float(GetScreenWidth()) / 2 - 125;
      start_rect.width = 250;
      start_rect.height = 80;
      if (GuiButton(start_rect, "START")) {
        return action::start;
      }
      start_rect.y += start_rect.height + 10;
      if (GuiButton(start_rect, "EXIT")) {
        return action::exit;
      }
      return action::none;
    }();

    [&]() {
      switch (current_action) {
        case action::start: m_should_exit = true; break;
        case action::exit: m_should_exit_game = true; break;
        case action::none: break;
      }
    }();
  }
} // namespace scenes
