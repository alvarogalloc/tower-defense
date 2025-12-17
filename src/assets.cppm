export module assets;
import std;
import debug;
import raylib;
import game;
Texture load_texture_cached(std::unordered_map<std::string, Texture>& cache,
                            std::string const& p);

Music load_music_cached(std::unordered_map<std::string, Music>& cache,
                        std::string const& p);

Sound load_sfx_cached(std::unordered_map<std::string, Sound>& cache,
                      std::string const& p);

export  {

enum class asset_type : std::uint8_t {
  texture,
  music,
  sfx,
};

class assets {
 public:
  template <asset_type AssetType>
  auto get(std::string_view logical_path) {
    using enum asset_type;
    using namespace std::filesystem;

    const std::string full_path = game::get().get_config().get_path(logical_path);
    debug::my_assert(exists(path{full_path}), std::format("Could not find asset {}", full_path));
    if constexpr (AssetType == texture) {
      return load_texture_cached(texture_cache, full_path);
    } else if constexpr (AssetType == music) {
      return load_music_cached(music_cache, full_path);
    } else if constexpr (AssetType == sfx) {
      return load_sfx_cached(sfx_cache, full_path);
    } else {
      static_assert(!sizeof(AssetType), "Unsupported asset type.");
    }
  }

 private:
  std::unordered_map<std::string, Texture> texture_cache;
  std::unordered_map<std::string, Music> music_cache;
  std::unordered_map<std::string, Sound> sfx_cache;
};

}
