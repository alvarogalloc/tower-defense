export module assets.test;
import ut;
import assets;
import sfml;
import stdbridge;

export void assets_test()
{
  using namespace ut;

  assets<sf::Texture> manager{ SRC_DIR "/assets" };
  int load_count = 0;

  manager.set_loader<sf::Texture>(
    [&](sf::Texture *tex, std::string_view path) { load_count++; });

  "assets"_test = [&] {
    auto texture = manager.get<sf::Texture>("characters.png");
    expect(manager.size<sf::Texture>() == 1_ul);

    auto sametexture = manager.get<sf::Texture>("characters.png");
    expect(manager.size<sf::Texture>() == 1_ul);
    manager.remove<sf::Texture>("characters.png");

    expect(1_i == load_count);

    auto texture2 = manager.get<sf::Texture>("characters2.png");
    expect(1_ul == manager.size<sf::Texture>());
    expect (2_i == load_count);
  };
}
