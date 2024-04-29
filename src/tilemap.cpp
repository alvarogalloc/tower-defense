module;
#include <cstddef>
#include <cassert>
module tilemap;

tilemap::tilemap(const tilemap_def &definition, sf::Texture *texture) {
  assert(texture != nullptr && "Texture is null");
  assert(definition.rows > 0 && "Rows must be greater than 0");
  assert(definition.columns > 0 && "Columns must be greater than 0");
  // assert(definition.tiles.size() == definition.rows * definition.columns &&
  //        "Tiles size must be equal to rows * columns");
  tileset = texture;
  create_map(definition);
}
void tilemap::create_map(const tilemap_def &definition) {
  // resize the vertex array to fit the level size
  vertices.setPrimitiveType(sf::Triangles);
  // this means two triangles per tile
  vertices.resize(definition.columns * definition.rows * 6);

  auto make_tile = [&](std::size_t i, std::size_t j) {
    // get the current tile number
    std::size_t tileNumber = definition.tiles[i + j * definition.columns] -definition.start_gid;
    if (tileNumber == 0)
      return;
    tileNumber--;

    // find its position in the tileset tileset
    const std::size_t n_tiles_per_row =
        static_cast<std::size_t>(tileset->getSize().x) /
        static_cast<std::size_t>(definition.tile_size.x);
    const std::size_t tileset_column = tileNumber % n_tiles_per_row;
    const std::size_t tileset_row = tileNumber / n_tiles_per_row;

    // get a pointer to the triangles' vertices of the current tile
    sf::Vertex *triangles = &vertices[(i + j * definition.columns) * 6];

    // define the 6 corners of the two triangles
    const sf::Vector2f first_position{
        static_cast<float>(i) * definition.tile_size.x,
        static_cast<float>(j) * definition.tile_size.y};

    triangles[0].position = first_position;
    triangles[1].position =
        first_position + sf::Vector2f{definition.tile_size.x, 0};
    triangles[2].position =
        first_position + sf::Vector2f{0, definition.tile_size.y};
    triangles[3].position = triangles[2].position;
    triangles[4].position = triangles[1].position;
    triangles[5].position = first_position + definition.tile_size;

    const sf::Vector2f total_separation = {
        static_cast<float>(tileset_column * definition.tile_separation),
        static_cast<float>(tileset_row * definition.tile_separation)};

    const auto start_position =
        sf::Vector2f{static_cast<float>(tileset_column) *
                         definition.tile_size.x,
                     static_cast<float>(tileset_row) * definition.tile_size.y} +
        total_separation;

    // first triangle
    triangles[0].texCoords = start_position;
    triangles[1].texCoords =
        start_position + sf::Vector2f{definition.tile_size.x, 0};
    triangles[2].texCoords =
        start_position + sf::Vector2f{0, definition.tile_size.y};
    // second triangle
    triangles[3].texCoords = triangles[2].texCoords;
    triangles[4].texCoords = triangles[1].texCoords;
    triangles[5].texCoords = start_position + definition.tile_size;
  };
  for (std::size_t i = 0; i < definition.columns; ++i)
    for (std::size_t j = 0; j < definition.rows; ++j)
      make_tile(i, j);
}

void tilemap::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  states.transform = getTransform();
  states.texture = tileset;
  target.draw(vertices, states);
}
