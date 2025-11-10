export module resource_manager;
import raylib;
import std;

export namespace systems {

/// Resource manager for automatic cleanup of game resources
class ResourceManager {
public:
    ResourceManager() = default;
    
    ~ResourceManager() {
        cleanup();
    }

    // Delete copy operations to prevent resource leaks
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Allow move operations
    ResourceManager(ResourceManager&& other) noexcept 
        : textures_(std::move(other.textures_))
        , sounds_(std::move(other.sounds_))
        , musics_(std::move(other.musics_)) {}
    
    ResourceManager& operator=(ResourceManager&& other) noexcept {
        if (this != &other) {
            cleanup();
            textures_ = std::move(other.textures_);
            sounds_ = std::move(other.sounds_);
            musics_ = std::move(other.musics_);
        }
        return *this;
    }

    /// Load and track a texture
    Texture2D load_texture(const char* path) {
        auto texture = LoadTexture(path);
        textures_.push_back(texture);
        return texture;
    }

    /// Load and track a sound
    Sound load_sound(const char* path) {
        auto sound = LoadSound(path);
        sounds_.push_back(sound);
        return sound;
    }

    /// Load and track music
    Music load_music(const char* path) {
        auto music = LoadMusicStream(path);
        musics_.push_back(music);
        return music;
    }

    /// Manually cleanup all resources
    void cleanup() {
        for (auto& texture : textures_) {
            UnloadTexture(texture);
        }
        textures_.clear();

        for (auto& sound : sounds_) {
            UnloadSound(sound);
        }
        sounds_.clear();

        for (auto& music : musics_) {
            UnloadMusicStream(music);
        }
        musics_.clear();
    }

private:
    std::vector<Texture2D> textures_;
    std::vector<Sound> sounds_;
    std::vector<Music> musics_;
};

} // namespace systems
