module assets;
namespace {

Texture load_texture_cached(std::unordered_map<std::string, Texture>& cache,
                            std::string const& p) {
  if (!cache.contains(p)) {
    std::println("loading texture {} and putting in cache", p);
    cache[p] = LoadTexture(p.c_str());
  } else {
    std::println("cache hit! texture {} is already loaded", p);
  }
  return cache[p];
}

Music load_music_cached(std::unordered_map<std::string, Music>& cache,
                        std::string const& p) {
  if (!cache.contains(p)) {
    std::println("loading music {} and putting in cache", p);
    cache[p] = LoadMusicStream(p.c_str());
  } else {
    std::println("cache hit! music {} is already loaded", p);
  }
  return cache[p];
}

Sound load_sfx_cached(std::unordered_map<std::string, Sound>& cache,
                      std::string const& p) {
  if (!cache.contains(p)) {
    std::println("loading sfx {} and putting in cache", p);
    cache[p] = LoadSound(p.c_str());
  } else {
    std::println("cache hit! sfx {} is already loaded", p);
  }
  return cache[p];
}

}  // namespace
