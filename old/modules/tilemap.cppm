export module tilemap;
import stdbridge;
import tmx;
import sfml;

class my_assets;
export
{
  class Tilemap : public sf::Drawable
  {

    void load_tilesets();

    void create_layer(tmx_layer *layer);

    void draw_object_layer(tmx_layer *layer);

    void create_layers();

  public:
    Tilemap() : m_map(nullptr, &tmx_map_free) {}
    Tilemap(std::string_view path, my_assets &manager);
    void load(std::string_view path, my_assets &manager);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    auto get_map() const -> const tmx_map * { return m_map.get(); }
    ~Tilemap();

  private:
    my_assets *m_manager;
    std::unordered_map<std::string_view, sf::Texture &> m_textures;
    std::unique_ptr<tmx_map, decltype(&tmx_map_free)> m_map;
    sf::RenderTexture m_render_texture;
  };
}
