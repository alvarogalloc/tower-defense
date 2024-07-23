module menu_scene;
import raygui;

void menu_scene::on_start()
{
  m_tilemap = std::make_unique<tilemap>(SRC_DIR "/assets/1.tmx");
  m_music = LoadMusicStream(SRC_DIR "/assets/bgmusic.ogg");
  // m_background = LoadTexture(SRC_DIR "/assets/background.png");
  m_title_font = LoadFont(SRC_DIR "/assets/monogram.ttf");
  // PlayMusicStream(*m_music);
  GuiSetFont(m_title_font);
  m_blue_guy = LoadTexture(SRC_DIR "/assets/blueguy.png");
}

void menu_scene::on_update()
{
  if (!IsMusicStreamPlaying(m_music)) { UpdateMusicStream(m_music); }
  if (IsKeyPressed(KEY_ENTER)) { m_should_exit = true; }
}

std::unique_ptr<scene> menu_scene::on_exit()
{
  StopMusicStream(m_music);

  UnloadMusicStream(m_music);
  UnloadFont(m_title_font);

  return nullptr;
}

void menu_scene::on_render()
{
  auto centerh = [](const Rectangle &rect) {
    auto width = GetScreenWidth();
    return Rectangle{
      float(width) / 2 - rect.width / 2, rect.y, rect.width, rect.height
    };
  };
  auto centerv = [](const Rectangle &rect) {
    auto height = GetScreenHeight();
    return Rectangle{
      rect.x, float(height) / 2 - rect.height / 2, rect.width, rect.height
    };
  };
  // DrawTexture(*m_background, 0, 0, colors::white);
  ClearBackground(colors::darkgreen);
  m_tilemap->draw();
  // calculate the width of the text
  auto text_width = MeasureTextEx(m_title_font, "Magster", 120, 0.2f).x;
  DrawTextEx(m_title_font,
    "Magster",
    Vector2{ float(GetScreenWidth()) / 2 - text_width / 2, 100 },
    120,
    0.2f,
    colors::yellow);

  DrawTextureEx(m_blue_guy,
    Vector2{ float(GetScreenWidth()) / 2.f - float(m_blue_guy.width) / 2.f,
      float(GetScreenHeight() - m_blue_guy.height * 3) },
    0,
    3,
    colors::white);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 96);
  if (GuiButton(centerv(centerh({ 10, 40, 240, 80 })), "START"))
  {
    m_should_exit = true;
  }
  GuiSetStyle(DEFAULT, TEXT_SIZE, 16);
  if (GuiButton(centerh({ 10, 400, 80, 40 }), "EXIT")) { m_should_exit = true; }
}
