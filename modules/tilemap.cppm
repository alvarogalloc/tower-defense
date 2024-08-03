
export module tilemap;
import stdbridge;
import raylib;
import tmx;

export
{

  class tilemap
  {
    std::unique_ptr<tmx_map, decltype(&tmx_map_free)> m_map;
    std::unordered_map<std::string_view, Texture2D> m_textures;
    RenderTexture2D m_texture{};

    void build_tilemap();
    void load_textures();

  public:
    tilemap(tmx_map *);
    tilemap(std::string_view path);
    auto get_map() const -> tmx_map * { return m_map.get(); }
    auto get_textures() const & { return m_textures; }
    auto get_target() const & { return m_texture; }

    void draw();
  };
}
