export module scenes.shooting;
import game;
import std;
import ginseng;
import raylib;

export namespace scenes {

  class shooting : public scene {
  std::vector<std::function<void(ginseng::database&, float)>> m_systems;
  std::vector<Texture2D> m_to_clean;
public:
    shooting();

    void on_start() override;
    void on_update() override;

    std::unique_ptr<scene> on_exit() override;
    void on_render() override;
  };
} // namespace scenes
