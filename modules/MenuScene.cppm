export module MenuScene;
import Game;
import sfml;
import stdbridge;


export class MenuScene : public Scene
{
public:
  virtual void on_start() override;
  virtual void on_event(const sf::Event&) override;
  virtual void on_update(float delta) override;
  virtual void on_render() override;
  virtual std::unique_ptr<Scene> on_exit() override;
};
