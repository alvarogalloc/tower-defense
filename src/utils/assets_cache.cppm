export module utils.assets_cache;
import std;
import debug;
import config;
import raylib;

using namespace std::filesystem;


// getters from cache
Texture get_texture(std::string const& p);
Music get_music(std::string const& p);
Sound get_sfx(std::string const& p);

export namespace utils {
enum class asset_type : std::uint8_t {
  texture,
  music,
  sfx,
};

// always valid paths, 

template <asset_type at>
auto get_asset(const std::string_view p_str) {
  using enum asset_type;

  auto p = config::get_path(p_str);
  if constexpr (at == texture) {
    return get_texture(p);
  } else if constexpr (at == music) {
    return get_music(p);
  } else if constexpr (at == sfx) {
    return get_sfx(p);
  } else {
    static_assert(false, "there is not a cache made for this one");
  }
}

}  // namespace rooster::utils
