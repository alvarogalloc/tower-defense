export module assets;
import std;
import debug;
import raylib;
import glaze;
namespace fs = std::filesystem;
template <typename T>
using Collection = std::unordered_map<std::string, T>;
Texture load_texture_cached(Collection<Texture>& cache, std::string const& p);

Music load_music_cached(Collection<Music>& cache, std::string const& p);
Sound load_sfx_cached(Collection<Sound>& cache, std::string const& p);

Font load_font_cached(Collection<Font>& cache, std::string const& p,
                      const int font_size);

export {
  enum class asset_type : std::uint8_t { texture, music, sfx, font };

  class assets {
    fs::path m_assets_path;

   public:
    assets(std::string_view assets_path_env_var);
    explicit assets(fs::path p) : m_assets_path(std::move(p)) {
      debug::my_assert(exists(p), "assets path does not exist");
    }

    template <typename pod_json_t>
    auto get_config(std::string_view logical_path) -> pod_json_t {
      using namespace std::filesystem;

      const std::string full_path = m_assets_path / logical_path;
      debug::my_assert(exists(path{full_path}),
                       std::format("Could not find config file {}", full_path));

      using glz::read_file_json, glz::format_error;
      std::string buf;
      pod_json_t data;
      auto ec = read_file_json(data, full_path, buf);
      if (ec) {
        const std::string error_msg{format_error(ec, buf)};
        debug::my_assert(false,
                         std::format("Error parsing configuration file({}): {}",
                                     full_path, error_msg));
      }
      return data;
    }
    template <asset_type AssetType, typename... Args>
    auto get(std::string_view logical_path, Args&&... args) {
      using enum asset_type;
      using namespace std::filesystem;

      const std::string full_path = m_assets_path / logical_path;
      debug::my_assert(exists(path{full_path}),
                       std::format("Could not find asset {}", full_path));
      if constexpr (AssetType == texture) {
        return load_texture_cached(texture_cache, full_path);
      } else if constexpr (AssetType == music) {
        return load_music_cached(music_cache, full_path);
      } else if constexpr (AssetType == sfx) {
        return load_sfx_cached(sfx_cache, full_path);
      } else if constexpr (AssetType == font) {
        return load_font_cached(font_cache, full_path,
                                std::forward<Args>(args)...);
      } else {
        static_assert(std::false_type{}, "Unsupported asset type.");
      }
    }

    ~assets();

   private:
    Collection<Texture> texture_cache;
    Collection<Music> music_cache;
    Collection<Sound> sfx_cache;
    Collection<Font> font_cache;
  };
}
