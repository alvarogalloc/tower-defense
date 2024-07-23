module tilemap;
import debug;
import fmt;


tilemap::tilemap(tmx_map *map) : m_map(map, &tmx_map_free)
{
  if (!m_map) { throw std::runtime_error("Invalid Map"); }
  load_textures();
  build_tilemap();
}

tilemap::tilemap(std::string_view path) : tilemap(tmx_load(path.data())) {}
void tilemap::draw()
{
  // RenderTexture2D target = build_tilemap(*this);
  DrawTextureRec(m_texture.texture,
    { 0,
      0,
      static_cast<float>(m_texture.texture.width),
      static_cast<float>(-m_texture.texture.height) },
    { 0, 0 },
    colors::white);
}


void tilemap::build_tilemap()
{
  m_texture = LoadRenderTexture(
    static_cast<int>(get_map()->width * get_map()->tile_width),
    static_cast<int>(get_map()->height * get_map()->tile_height));
  auto *current_layer = m_map->ly_head;
  auto paint_layer = [&](tmx_layer *layer) {
    for (auto i : std::views::iota(0u, m_map->width))
    {
      for (auto j : std::views::iota(0u, m_map->height))
      {

        const auto gid =
          layer->content.gids[(i * m_map->width) + j] & tmx_flip_bits_removal;
        if (m_map->tiles[gid] == nullptr) { continue; }
        Rectangle tile_rect(float(m_map->tiles[gid]->ul_x),
          float(m_map->tiles[gid]->ul_y),
          float(m_map->tiles[gid]->tileset->tile_width),
          float(m_map->tiles[gid]->tileset->tile_height));
        const auto texture = m_textures.at(m_map->tiles[gid]->tileset->name);
        Vector2 pos{ static_cast<float>(
                       j * m_map->tiles[gid]->tileset->tile_width),
          static_cast<float>(i * m_map->tiles[gid]->tileset->tile_height) };

        DrawTextureRec(texture, tile_rect, pos, colors::white);
      }
    }
  };
  auto paint_objects = [&](tmx_layer *layer) {
    auto current_object = layer->content.objgr->head;
    const auto rand_color =
      Color{ static_cast<unsigned char>(GetRandomValue(0, 255)),
        static_cast<unsigned char>(GetRandomValue(0, 1)),
        static_cast<unsigned char>(GetRandomValue(0, 255)),
        255 };
    while (current_object)
    {
      if (current_object->obj_type == OT_POINT)
      {
        DrawCircle(static_cast<int>(current_object->x),
          static_cast<int>(current_object->y),
          5,
          rand_color);
      }
      current_object = current_object->next;
    }
  };
  my_assert(IsRenderTextureReady(m_texture), "RenderTexture not ready");
  BeginTextureMode(m_texture);
  ClearBackground(colors::white);
  // go through all the layers
  while (current_layer)
  {
    switch (current_layer->type)
    {
    case L_NONE:
    case L_IMAGE:
    case L_GROUP:
      fmt::print(warn, "layer type not supported\n");
      break;
    case L_LAYER:
      paint_layer(current_layer);
      break;
    case L_OBJGR:
      paint_objects(current_layer);
      break;
    }
    current_layer = current_layer->next;
  }

  // it is upside down, so we need to flip it
  EndTextureMode();
}
void tilemap::load_textures()
{
  auto head = get_map()->ts_head;
  while (head)
  {
    auto realpath =
      fmt::format("{}/assets/{}", SRC_DIR, head->tileset->image->source);
    my_assert(std::filesystem::exists(realpath),
      fmt::format("File does not exist {}", realpath));
    auto texture = LoadTexture(realpath.c_str());
    m_textures.emplace(head->tileset->name, texture);
    head = head->next;
  }
}
