module MenuScene;
import GameScene;
import fmt;
import utils;


void MenuScene::on_start()
{
  music = m_manager->get<sf::Music>("bgmusic.mp3");
  music->setLoop(true);
  music->setVolume(30);
  // music->play();
  title_font = m_manager->get<sf::Font>("monogram.ttf");
  title_text.setFont(*title_font);
  title_text.setCharacterSize(128);
  title_text.setString("MAGSTER");
  // set in the middle of the screen
  const sf::Vector2f title_pos{
    m_win->getSize().x / 2.f - title_text.getGlobalBounds().width / 2.f, 50
  };
  title_text.setPosition(title_pos);
  title_text.setFillColor(sf::Color(50, 135, 68));
}
void MenuScene::on_update(float delta) {}
void MenuScene::on_event(const sf::Event &e) {}
void MenuScene::on_render()
{


  // shouldn't matter, as imgui is always on top
  m_win->clear(sf::Color(17, 35, 77));
  m_win->draw(title_text);
}

std::unique_ptr<Scene> MenuScene::on_exit()
{
  music->stop();
  return std::make_unique<GameScene>();
}
