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
    m_music = LoadMusicStream(routes::MENU_MUSIC);
    PlayMusicStream(m_music);
  }

  void menu::on_update()
  {
    m_space_background.update(GetFrameTime());
    UpdateMusicStream(m_music);
  }

  std::unique_ptr<scene> menu::on_exit()
  {
    UnloadTexture(m_blue_guy);
    // UnloadFont(m_title_font); font is shared with raygui so we don't unload it
    return std::make_unique<scenes::shooting>();
  }

  void menu::on_render()
  {
    m_space_background.draw();

    float next_element_y = 0;
    const int font_size = 96;
    [&]() {
      constexpr static std::string_view title = "Rooster\nSpace";
      const auto text_size = MeasureTextEx(m_title_font, title.data(), 120, 0.2f);
      const auto text_pos = Vector2 {.x = float(GetScreenWidth()) / 2 - text_size.x / 2, .y = 100};
      // draw a brown rectangle behind the text
      auto title_rect = Rectangle {text_pos.x - 10, text_pos.y, text_size.x + 10, text_size.y + 10};
      DrawRectangleRec(title_rect, colors::brown);
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
      GuiSetStyle(DEFAULT, TEXT_SIZE, font_size);
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
