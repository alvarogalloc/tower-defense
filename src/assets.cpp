module;
#include <cstdlib>
module assets;

Texture load_texture_cached(Collection<Texture>& cache, std::string const& p) {
  if (!cache.contains(p)) {
    std::println("loading texture {} and putting in cache", p);
    cache[p] = LoadTexture(p.c_str());
  } else {
    std::println("cache hit! texture {} is already loaded", p);
  }
  return cache[p];
}

Music load_music_cached(Collection<Music>& cache, std::string const& p) {
  if (!cache.contains(p)) {
    std::println("loading music {} and putting in cache", p);
    cache[p] = LoadMusicStream(p.c_str());
  } else {
    std::println("cache hit! music {} is already loaded", p);
  }
  return cache[p];
}

Sound load_sfx_cached(Collection<Sound>& cache, std::string const& p) {
  if (!cache.contains(p)) {
    std::println("loading sfx {} and putting in cache", p);
    cache[p] = LoadSound(p.c_str());
  } else {
    std::println("cache hit! sfx {} is already loaded", p);
  }
  return cache[p];
}

Font load_font_cached(Collection<Font>& cache, std::string const& p,
                      const int font_size) {
  if (!cache.contains(p)) {
    std::println("loading font {} and putting in cache", p);
    cache[p] = LoadFontEx(p.c_str(), font_size, nullptr, 0);
  } else {
    std::println("cache hit! sfx {} is already loaded", p);
  }
  return cache[p];
}

assets::assets(std::string_view assets_path_env_var) {
  const char* assets_path = ::getenv(assets_path_env_var.data());
  namespace fs = std::filesystem;

  debug::my_assert(
      assets_path,
      std::format("the assets path environment variable ({}) does not exist, set it to load your assets",
                  assets_path_env_var));
  const fs::path p{assets_path};
  debug::my_assert(
      fs::exists(p),
      std::format("path to assets does not exist ({})", p.string()));
  std::println("setting path to {}", fs::canonical(p).string());
  m_assets_path = fs::canonical(p);
}

assets::~assets() {
  for (const auto& [p, m] : music_cache) {
    UnloadMusicStream(m);
    std::println("Unloaded music {}", p);
  }
  for (const auto& [p, s] : sfx_cache) {
    UnloadSound(s);
    std::println("Unloaded sfx {}", p);
  }
  for (const auto& [p, t] : texture_cache) {
    UnloadTexture(t);
    std::println("Unloaded texture {}", p);
  }
}
