export module assets;
/*
import stdbridge;
import raylib;
import fmt;

constexpr auto warn = fmt::emphasis::bold | fmt::fg(fmt::color::yellow);
constexpr auto error = fmt::emphasis::bold | fmt::fg(fmt::color::red);
constexpr auto info = fmt::emphasis::bold | fmt::fg(fmt::color::cyan);
constexpr auto success = fmt::emphasis::bold | fmt::fg(fmt::color::green);


template<typename... T> class assets
{
public:
  template<typename Contained>
  using Map = std::unordered_map<std::string_view, std::unique_ptr<Contained>>;
  template<typename Contained>
  using Loader = std::function<void(Contained *, std::string_view)>;

  template<typename Contained>
  using unLoader = std::function<void(Contained *)>;

  constexpr explicit assets(std::string_view path) : m_path(path)
  {
    namespace fs = std::filesystem;
    if (!fs::exists(m_path))
    {
      throw std::runtime_error("Path does not exist");
    }
    if (!m_path.ends_with('/')) { m_path += '/'; }
  }
  constexpr auto get_path() const { return m_path; }

  template<typename U>
  constexpr void set_loader(Loader<U> loader)
    requires(std::is_same_v<U, T> || ...)
  {
    std::get<Loader<U>>(m_loaders) = loader;
  }

  template<typename U>
  constexpr void set_unloader(unLoader<U> unloader)
    requires(std::is_same_v<U, T> || ...)
  {
    std::get<unLoader<U>>(m_unloaders) = unloader;
  }


  template<typename U>
  [[nodiscard]] constexpr U *get(std::string_view name)
    requires(std::is_same_v<U, T> || ...)
  {
    if (auto it = std::get<Map<U>>(m_assets).find(name);
        it != std::get<Map<U>>(m_assets).end())
    {
      fmt::print(info, "Found in cache '{}'\n", name);
      return it->second.get();
    }
    fmt::print(success, "Inserting '{}'\n", name);
    auto [it, success] =
      std::get<Map<U>>(m_assets).try_emplace(name, std::make_unique<U>());
    if (!success) { throw std::runtime_error("Failed to insert asset"); }

    // check if loader is set
    if (!std::get<Loader<U>>(m_loaders))
    {
      throw std::runtime_error(
        fmt::format("Loader for this type is not set, please set "
                    "using the set_loader<{}> method",
          typeid(U).name()));
    }
    std::invoke(
      std::get<Loader<U>>(m_loaders), it->second.get(), m_path + name.data());
    return it->second.get();
  }

  template<typename U>
  constexpr void remove(std::string_view name)
    requires(std::is_same_v<U, T> || ...)
  {
    auto it = std::get<Map<U>>(m_assets).find(name);
    my_assert(it != std::get<Map<U>>(m_assets).end(),
      fmt::format("Asset '{}' not found", name));
    auto should_delete =
      it != std::get<Map<U>>(m_assets).end();
      // && std::find(prevent_deletion.begin(), prevent_deletion.end(), name)
      //      == prevent_deletion.end();
    if (should_delete)
    {
      std::invoke(std::get<unLoader<U>>(m_unloaders), it->second.get());
      fmt::print(warn, "Erasing '{}'\n", name);
      std::get<Map<U>>(m_assets).erase(it);
    }
  }

  // get amount of assets
  template<typename U>
  [[nodiscard]] constexpr auto size() const
    requires(std::is_same_v<U, T> || ...)
  {
    return std::get<Map<U>>(m_assets).size();
  }
  ~assets()
  {
    // call unload all assets
    std::apply(
      [this](Map<T> &...maps) {
        (
          [this](const auto &map) {
            for (auto &[key, val] : map)
            {
              fmt::print(warn, "Erasing '{}'\n", key);
              std::invoke(std::get<unLoader<T>>(m_unloaders), val.get());
            }
          }(maps),
          ...);
      },
      m_assets);
  }

protected:
  // Member variables declared but not defined
  std::tuple<Map<T>...> m_assets;
  std::tuple<Loader<T>...> m_loaders;
  std::tuple<unLoader<T>...> m_unloaders;
  std::string m_path;
};


export class my_assets : public assets<Texture2D, Music, Font>
{
public:
  my_assets(std::string_view path) : assets(path)
  {
    set_loader<Texture2D>([](Texture2D *tex, std::string_view path) {
      *tex = LoadTexture(path.data());
      if (tex->id == 0) { throw std::runtime_error("Failed to load texture"); }
    });
    set_loader<Music>([](Music *music, std::string_view path) {
      *music = LoadMusicStream(path.data());
      if (music->ctxType == 0)
      {
        throw std::runtime_error("Failed to load music");
      }
    });
    set_loader<Font>([](Font *font, std::string_view path) {
      *font = LoadFont(path.data());
      if (font->baseSize == 0)
      {
        throw std::runtime_error("Failed to load font");
      }
    });

    set_unloader<Texture2D>([](Texture2D *tex) { UnloadTexture(*tex); });
    set_unloader<Music>([](Music *music) { UnloadMusicStream(*music); });
    set_unloader<Font>([](Font *font) { UnloadFont(*font); });
  }
};
*/