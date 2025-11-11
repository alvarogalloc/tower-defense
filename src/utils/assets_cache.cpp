module utils.assets_cache;

Texture get_texture(std::string const& p) {
  static std::unordered_map<std::string, Texture> cache;
  if (!cache.contains(p)) {
    std::println("loading texture {} and putting it cache", p);
    cache[p] = LoadTexture(p.c_str());
  } 
  else {

    std::println("cache hit! texture {} is loaded", p);
  }
  return cache[p];
}
Music get_music(std::string const& p) {
  static std::unordered_map<std::string, Music> cache;
  if (!cache.contains(p)) {
    std::println("loading music {} and putting it cache", p);
    cache[p] = LoadMusicStream(p.c_str());
  }
  else {

    std::println("cache hit! music {} is loaded", p);
  }
  return cache[p];
}
Sound get_sfx(std::string const& p) {
  static std::unordered_map<std::string, Sound> cache;
  if (!cache.contains(p)) {
    std::println("loading sfx {} and putting it cache", p);
    cache[p] = LoadSound(p.c_str());
  }
  else {

    std::println("cache hit! sfx {} is loaded", p);
  }
  return cache[p];
}
