export module assets;
import sfml;
import say;
import stdbridge;

export
{
    class assets;

    template<typename T>
    concept asset_like = requires(T t) {
        { t.loadFromFile("") } -> std::convertible_to<bool>;
    };
    template<typename T>
    concept stream_like = requires(T t) {
        { t.openFromFile("") } -> std::convertible_to<bool>;
    };

    template<typename T>
    concept loadable = stream_like<T> || asset_like<T>;

    class assets
    {
        template<typename T> using ptr = std::shared_ptr<T>;

        template<typename T>
        using asset_storage = std::unordered_map<std::string_view, ptr<T>>;


      public:
        explicit assets(std::string_view _asset_path) : asset_path(_asset_path)
        {
            namespace fs = std::filesystem;
            if (!fs::exists(asset_path)) { say::error("Path does not exist"); }
            if (!asset_path.ends_with('/')) { asset_path += '/'; }
        }
        template<loadable T> [[nodiscard]] T* get(std::string_view path)
        {
            static_assert(std::is_same_v<T, sf::Texture>
                            || std::is_same_v<T, sf::Music>
                            || std::is_same_v<T, sf::Font>,
              "Asset Type is not supported");

            asset_storage<T> *target{ nullptr };
            if constexpr (std::is_same_v<T, sf::Texture>)
            {
                target = &textures;
            }
            else if constexpr (std::is_same_v<T, sf::Music>)
                {
                    target = &music;
                }
            else if constexpr (std::is_same_v<T, sf::Font>)
            {
                target = &fonts;
            }

            if (!target->contains(path))
            {
                if (!load(path, *target))
                {
                    throw std::runtime_error{ fmt::format(
                      "could not load {}", path) };
                }
            }
            return target->at(path).get();
        }

      private:
        template<stream_like T>
        bool load(std::string_view path, asset_storage<T> &storage)
        {
            auto [res, ok] = storage.insert({ path, std::make_shared<T>() });
            if (!ok) { return false; }
            ok = res->second->openFromFile(path.data());
            return ok;
        }
        template<asset_like T>
        bool load(std::string_view path, asset_storage<T> &storage)
        {
            auto [res, ok] = storage.insert({ path, std::make_shared<T>() });
            if (!ok) { return false; }
            ok = res->second->loadFromFile(asset_path + path.data());
            return ok;
        }


        asset_storage<sf::Texture> textures;
        asset_storage<sf::Music> music;
        asset_storage<sf::Font> fonts;
        std::string asset_path;
    };
}
