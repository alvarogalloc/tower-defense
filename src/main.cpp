import sfml;
import fmt;
import stdutils;
import tmx;
import assets;
import stringables;

const sf::Vector2u win_size{640, 640};

constexpr auto font_path = "/System/Library/Fonts/Times.ttc";

class MapLayer : public sf::Drawable {
  // Takes ownership of the tmx_map
  void load_tilesets(assets &manager) {
    auto head = m_map->ts_head;
    while (head) {
      fmt::println("loading tileset {} from {}", head->tileset->name,
                   head->tileset->image->source);
      auto texture = manager.get<sf::Texture>(head->tileset->image->source);
      m_textures.emplace(head->tileset->name, texture);
      head = head->next;
    }
  }

  void create_layer(tmx_layer *layer) {
    if (layer->type != L_LAYER)
      return;
    sf::Sprite sprite;
    sf::Texture *current_texture;
    for (std::size_t i = 0; i < m_map->width; i++) {
      for (std::size_t j = 0; j < m_map->height; j++) {
        const auto gid =
            layer->content.gids[(i * m_map->width) + j] & tmx_flip_bits_removal;
        if (m_map->tiles[gid] == nullptr) {
          continue;
        }
        current_texture = m_textures.at(m_map->tiles[gid]->tileset->name).get();
        sprite.setTexture(*current_texture);
        sprite.setTextureRect(
            sf::IntRect(m_map->tiles[gid]->ul_x, m_map->tiles[gid]->ul_y,
                        m_map->tiles[gid]->tileset->tile_width,
                        m_map->tiles[gid]->tileset->tile_height));
        sprite.setPosition(j * m_map->tiles[gid]->tileset->tile_width,
                           i * m_map->tiles[gid]->tileset->tile_height);
        m_render_texture.draw(sprite);
      }
    }
  }
  void draw_object_layer(tmx_layer *layer) {
    if (layer->type != L_OBJGR)
      return;

    sf::RectangleShape rect;
    auto current_object = layer->content.objgr->head;
    while (current_object) {
      rect.setSize({20, 20});
      rect.setPosition(current_object->x, current_object->y);
      rect.setFillColor(sf::Color::Red);
      m_render_texture.draw(rect);
      current_object = current_object->next;
    }
  }
  void create_layers() {
    m_render_texture.create(m_map->width * m_map->tile_width,
                            m_map->height * m_map->tile_height);
    constexpr auto ARGB_to_RGBA = [](std::uint32_t color) -> sf::Color {
      sf::Color c;
      c.r = (color & 0xff0000) >> 16;
      c.g = (color & 0xff00) >> 8;
      c.b = color & 0xff;
      c.a = (color & 0xff000000) >> 24;
      return c;
    };
    if (m_map->backgroundcolor != 0) {

      m_render_texture.clear(ARGB_to_RGBA(m_map->backgroundcolor));
    } else {

      m_render_texture.clear();
    }
    auto current_layer = m_map->ly_head;
    while (current_layer) {
      switch (current_layer->type) {
      case L_LAYER:
        create_layer(current_layer);
        break;
      case L_OBJGR:
        draw_object_layer(current_layer);
        break;
      default:
        fmt::println("unsupported layer type");
        break;
      }
      current_layer = current_layer->next;
    }
  }

public:
  MapLayer(std::unique_ptr<tmx_map, decltype(&tmx_map_free)> map,
           assets &manager)
      : m_map{std::move(map)} {
    load_tilesets(manager);
    create_layers();
  }
  MapLayer(std::string_view path, assets &manager)
      : m_map{tmx_load(path.data()), tmx_map_free} {
    load_tilesets(manager);
    create_layers();
  }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override {
    sf::Sprite sprite(m_render_texture.getTexture());
    // it is drawing upside down
    sprite.setScale(1, -1);
    sprite.setPosition(0, m_map->height * m_map->tile_height);
    target.draw(sprite, states);
  }

private:
  std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_textures;
  std::unique_ptr<tmx_map, decltype(&tmx_map_free)> m_map;
  sf::RenderTexture m_render_texture;
};

int main(int argc, char *argv[]) {
  sf::RenderWindow win{sf::VideoMode{win_size.x, win_size.y}, "window"};

  const auto middle_screen = sf::VideoMode::getDesktopMode().width / 2;
  win.setPosition({static_cast<int>(middle_screen), 0});

  win.setFramerateLimit(60);

  assets manager{"./assets/tiled"};

  MapLayer layer{"./assets/tiled/1.tmx", manager};

  sf::Event ev;
  while (win.isOpen()) {
    while (win.pollEvent(ev)) {
      if (ev.type == sf::Event::Closed) {
        win.close();
      }
    }
    win.clear();
    win.draw(layer);
    win.display();
  }
}
