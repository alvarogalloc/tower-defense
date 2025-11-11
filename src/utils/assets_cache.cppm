export module utils.assets_cache;
import std;
import debug;
import raylib;

using namespace std::filesystem;

export namespace utils {

struct asset_path : path {
  asset_path(path const& p) : std::filesystem::path(p) {
    debug::my_assert(exists(p), std::format("{} does not exist", p.string()));
  }
};
}  // namespace rooster::utils

// getters from cache
using namespace utils;
Texture get_texture(asset_path const& p);
Music get_music(asset_path const& p);
Sound get_sfx(asset_path const& p);

export namespace utils {
enum class asset_type : std::uint8_t {
  texture,
  music,
  sfx,
};

// always valid paths, 

template <asset_type at>
auto get_asset(const asset_path& p) {
  using enum asset_type;
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
