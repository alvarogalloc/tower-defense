export module event_system;
import std;
import raylib;

export namespace systems {

/// Base event type - all events should derive from this
struct Event {
    virtual ~Event() = default;
};

/// Event: Player wants to shoot
struct ShootEvent : Event {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    int damage;
};

/// Event: Enemy was killed
struct EnemyKilledEvent : Event {
    int score_value;
};

/// Event: Player took damage
struct PlayerDamagedEvent : Event {
    int damage;
};

/// Event system for decoupled inter-system communication
class EventBus {
public:
    using EventHandler = std::function<void(const Event&)>;

    /// Subscribe to events of a specific type
    template<typename EventType>
    void subscribe(EventHandler handler) {
        auto type_id = std::type_index(typeid(EventType));
        handlers_[type_id].push_back(std::move(handler));
    }

    /// Publish an event to all subscribers
    template<typename EventType>
    void publish(const EventType& event) {
        auto type_id = std::type_index(typeid(EventType));
        auto it = handlers_.find(type_id);
        if (it != handlers_.end()) {
            for (const auto& handler : it->second) {
                handler(event);
            }
        }
    }

    /// Clear all event handlers
    void clear() {
        handlers_.clear();
    }

private:
    std::unordered_map<std::type_index, std::vector<EventHandler>> handlers_;
};

} // namespace systems
