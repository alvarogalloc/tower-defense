export module assets;
import sfml;
import say;
import stdbridge;
import tilemap;

export
{
  template<typename... T> class assets
  {


  public:
    template<typename Contained>
    using Map =
      std::unordered_map<std::string_view, std::unique_ptr<Contained>>;
    template<typename Contained>
    using Loader = std::function<void(Contained *, std::string_view)>;
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
    [[nodiscard]] constexpr U *get(std::string_view name)
      requires(std::is_same_v<U, T> || ...)
    {
      if (auto it = std::get<Map<U>>(m_assets).find(name);
          it != std::get<Map<U>>(m_assets).end())
      {
        return it->second.get();
      }
      auto [it, success] =
        std::get<Map<U>>(m_assets).insert({ name, std::make_unique<U>() });

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

    template<typename U> constexpr void remove(std::string_view name)
      requires(std::is_same_v<U, T> || ...)
    {
      if (auto it = std::get<Map<U>>(m_assets).find(name);
          it != std::get<Map<U>>(m_assets).end())
      {
        std::get<Map<U>>(m_assets).erase(it);
      }
    }

  protected:
    // Member variables declared but not defined
    std::tuple<Map<T>...> m_assets;
    std::tuple<Loader<T>...> m_loaders;
    std::string m_path;
  };

  // instantiate the template for sfml types
  class my_assets : public assets<sf::Texture, sf::Font, sf::Music, Tilemap>
  {
  public:
    my_assets(std::string_view path);
  };
}
