export module tilemap;
import stdbridge;
import tmx;
import sfml;

class my_assets;
export
{
    class Tilemap : public sf::Drawable
    {

        void load_tilesets(my_assets &manager);

        void create_layer(tmx_layer *layer);

        void draw_object_layer(tmx_layer *layer);

        void create_layers();

      public:
        Tilemap() :m_map(nullptr, &tmx_map_free) {}
        Tilemap(std::unique_ptr<tmx_map, decltype(&tmx_map_free)> map,
          my_assets &manager);

        Tilemap(std::string_view path, my_assets &manager);
        void load(std::string_view path, my_assets &manager);
        void draw(sf::RenderTarget &target,
          sf::RenderStates states) const override;
        auto get_map() const -> const tmx_map * { return m_map.get(); }
        ~Tilemap() {
            m_textures.clear();
            m_map.reset();

        }

      private:
        std::unordered_map<std::string, sf::Texture &> m_textures;
        std::unique_ptr<tmx_map, decltype(&tmx_map_free)> m_map;
        sf::RenderTexture m_render_texture;
    };
}
