export module tilemap;
import stdutils;
import tmx;
import sfml;

struct assets;

export
{
    class Tilemap : public sf::Drawable
    {

        void load_tilesets(assets &manager);

        void create_layer(tmx_layer *layer);

        void draw_object_layer(tmx_layer *layer);

        void create_layers();

      public:
        Tilemap(std::unique_ptr<tmx_map, decltype(&tmx_map_free)> map, assets &manager);

        Tilemap(std::string_view path, assets &manager);
        void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

      private:
        std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_textures;
        std::unique_ptr<tmx_map, decltype(&tmx_map_free)> m_map;
        sf::RenderTexture m_render_texture;
    };
}
