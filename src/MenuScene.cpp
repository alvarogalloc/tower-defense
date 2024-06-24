module MenuScene;
import GameScene;
import imgui;
import fmt;


void MenuScene::on_start() {}
void MenuScene::on_update(float delta) {}
void MenuScene::on_event(const sf::Event &e) {}
void MenuScene::on_render()
{
  ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
  enum class menu_state { none, start, credits, exit };
  const auto win_size = m_win->getSize();
  auto TextCentered = [](std::string text) {
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text("%s", text.c_str());
  };

  const auto menu_width = win_size.x / 2.f;
  ImGui::SetNextWindowSize(ImVec2(menu_width, win_size.y));
  ImGui::SetNextWindowPos(
    ImVec2((ImGui::GetIO().DisplaySize.x / 2.f) - (menu_width / 2.f),
      win_size.y / 4.f));
  menu_state state = menu_state::none;

  if (ImGui::Begin("Start Menu",
        nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
          | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration
          | ImGuiWindowFlags_NoBackground))
  {


    TextCentered("Tower Defense");
    ImGui::SetCursorPosY(50);
    if (ImGui::Button("Start", ImVec2(menu_width, 50)))
    {
      // Handle start button click
      state = menu_state::start;
    }
    ImGui::SetCursorPosY(120);
    if (ImGui::Button("Credits", ImVec2(menu_width, 50)))
    {
      // Handle credits button click
      state = menu_state::credits;
    }
    ImGui::SetCursorPosY(190);
    if (ImGui::Button("Exit", ImVec2(menu_width, 50)))
    {
      state = menu_state::exit;
    }

    ImGui::End();
  }
  ImGui::PopFont();


  switch (state)
  {
  case menu_state::start:
    m_should_exit = true;
    break;
  case menu_state::credits:
    fmt::println("credits");
    break;
  case menu_state::exit:
    m_win->close();
    break;
  default:
    break;
  }

  m_win->clear(sf::Color::Cyan);
}

std::unique_ptr<Scene> MenuScene::on_exit()
{
  return std::make_unique<GameScene>();
}
