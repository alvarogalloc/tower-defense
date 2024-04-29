module;
#include <cstddef>
#include <vector>
export module tilemap;
import sfml;

export {
  struct tilemap_def {
    std::size_t rows;
    std::size_t columns;
    std::size_t tile_separation;
    sf::Vector2f tile_size;
    std::vector<std::size_t> tiles;
  std::size_t start_gid = 0;
  };

  struct tilemap : public sf::Drawable, public sf::Transformable {
    tilemap(const tilemap_def &definition, sf::Texture *texture);
    void create_map(const tilemap_def &definition);
    virtual void draw(sf::RenderTarget &target,
                      sf::RenderStates states) const override;

    sf::Texture *tileset;
    sf::VertexArray vertices;
  };
}
