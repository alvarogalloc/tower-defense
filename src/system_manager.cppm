export module system_manager;
import ginseng;
import std;

export namespace systems {

/// Lifecycle phases for system execution
enum class Phase {
    Init,      // One-time initialization
    Update,    // Per-frame update logic
    Cleanup    // Cleanup/shutdown
};

/// System function signature - takes database, delta time, and phase
using SystemFunc = std::function<void(ginseng::database&, float, Phase)>;

/// Manages registration and execution of all game systems
class SystemManager {
public:
    /// Register a system with a given priority (lower = earlier execution)
    void register_system(std::string_view name, SystemFunc func, int priority = 100) {
        systems_.emplace_back(SystemEntry{
            .name = std::string(name),
            .func = std::move(func),
            .priority = priority,
            .enabled = true
        });
        // Keep systems sorted by priority
        std::ranges::sort(systems_, [](const auto& a, const auto& b) {
            return a.priority < b.priority;
        });
    }

    /// Execute all enabled systems for the given phase
    void execute(ginseng::database& db, float dt, Phase phase) {
        for (auto& system : systems_) {
            if (system.enabled) {
                system.func(db, dt, phase);
            }
        }
    }

    /// Enable or disable a specific system by name
    void set_enabled(std::string_view name, bool enabled) {
        for (auto& system : systems_) {
            if (system.name == name) {
                system.enabled = enabled;
                return;
            }
        }
    }

    /// Clear all registered systems
    void clear() {
        systems_.clear();
    }

private:
    struct SystemEntry {
        std::string name;
        SystemFunc func;
        int priority;
        bool enabled;
    };

    std::vector<SystemEntry> systems_;
};

} // namespace systems
