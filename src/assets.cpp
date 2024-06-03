module assets;
import tilemap;


my_assets::my_assets(std::string_view path) : assets(path)
{
  set_loader<sf::Texture>([](sf::Texture *texture, std::string_view path) {
    if (!texture->loadFromFile(path.data()))
    {
      throw std::runtime_error("Failed to load texture");
    }
  });
  set_loader<sf::Font>([](sf::Font *font, std::string_view path) {
    if (!font->loadFromFile(path.data()))
    {
      throw std::runtime_error("Failed to load font");
    }
  });
  set_loader<sf::Music>([](sf::Music *music, std::string_view path) {
    if (!music->openFromFile(path.data()))
    {
      throw std::runtime_error("Failed to load music");
    }
  });
  set_loader<Tilemap>([this](Tilemap *tilemap, std::string_view path) {
    tilemap->load(path, *this);
  });
}
