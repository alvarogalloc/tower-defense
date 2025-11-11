module utils.assets_cache;

using namespace utils;

Texture get_texture(asset_path const& p) {
  static std::unordered_map<std::string, Texture> cache;
  if (!cache.contains(p.string())) {
    std::println("loading texture {} and putting it cache", p.string());
    cache[p.string()] = LoadTexture(p.c_str());
  } 
  else {

    std::println("cache hit! texture {} is loaded", p.string());
  }
  return cache[p];
}
Music get_music(asset_path const& p) {
  static std::unordered_map<std::string, Music> cache;
  if (!cache.contains(p.string())) {
    std::println("loading music {} and putting it cache", p.string());
    cache[p.string()] = LoadMusicStream(p.c_str());
  }
  else {

    std::println("cache hit! music {} is loaded", p.string());
  }
  return cache[p];
}
Sound get_sfx(asset_path const& p) {
  static std::unordered_map<std::string, Sound> cache;
  if (!cache.contains(p.string())) {
    std::println("loading sfx {} and putting it cache", p.string());
    cache[p.string()] = LoadSound(p.c_str());
  }
  else {

    std::println("cache hit! sfx {} is loaded", p.string());
  }
  return cache[p];
}
