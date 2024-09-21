module menu_scene;
import raygui;
import ellipse_scene;
import asset_routes;

void menu_scene::on_start()
{
  using namespace asset_routes;
  m_title_font = LoadFont(TITLE_FONT);
  GuiSetFont(m_title_font);
  m_blue_guy = LoadTexture(BLUE_GUY_TEXTURE);
}

void menu_scene::on_update()
{
  if (IsKeyPressed(KEY_ENTER)) {
    m_should_exit = true;
  }
}

std::unique_ptr<scene> menu_scene::on_exit()
{
  UnloadTexture(m_blue_guy);
  // UnloadFont(m_title_font); font is shared with raygui so we don't unload it
  return std::make_unique<ellipse_scene>();
}

void menu_scene::on_render()
{
  auto centerh = [](const Rectangle& rect) {
    auto width = GetScreenWidth();
    return Rectangle {float(width) / 2 - rect.width / 2, rect.y, rect.width, rect.height};
  };
  auto centerv = [](const Rectangle& rect) {
    auto height = GetScreenHeight();
    return Rectangle {rect.x, float(height) / 2 - rect.height / 2, rect.width, rect.height};
  };
  // DrawTexture(*m_background, 0, 0, colors::white);
  ClearBackground(colors::darkgreen);
  // calculate the width of the text
  const auto text_width = MeasureTextEx(m_title_font, "Magster", 120, 0.2f).x;
  const auto text_pos = Vector2 {.x = float(GetScreenWidth()) / 2 - text_width / 2, .y = 100};
  // draw a brown rectangle behind the text
  DrawRectangleRec(Rectangle {text_pos.x - 10, text_pos.y, text_width + 10, 120}, colors::brown);
  DrawTextEx(m_title_font, "Magster", text_pos, 120, 0.2f, colors::yellow);

  // DrawTextureEx(m_blue_guy,
  //   Vector2{ float(GetScreenWidth()) / 2.f - float(m_blue_guy.width) / 2.f,
  //     float(GetScreenHeight() - m_blue_guy.height * 3) },
  //   0,
  //   3,
  //   colors::white);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 96);
  if (GuiButton(centerv(centerh({10, 40, 240, 80})), "START")) {
    m_should_exit = true;
  }
  GuiSetStyle(DEFAULT, TEXT_SIZE, 16);
  if (GuiButton(centerh({10, 400, 80, 40}), "EXIT")) {
    CloseWindow();
  }
}
