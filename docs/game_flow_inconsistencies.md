# Game Flow Inconsistencies Analysis

## Executive Summary

This document identifies the key inconsistencies in the tower defense game's architecture and proposes solutions using proper ECS (Entity Component System) patterns with the ginseng library.

## Identified Inconsistencies

### 1. Mixed System Organization Pattern

**Problem:**
```cpp
class shooting : public scene {
    // Some systems stored in vector
    std::vector<std::function<void(ginseng::database&, float)>> m_systems{};
    
    void on_update() {
        // Some systems called from vector
        for (auto& system : m_systems) {
            system(world, dt);
        }
        
        // Other systems called directly
        systems::bullet::update(world, dt, cam);
    }
};
```

**Issues:**
- Inconsistent execution pattern
- Hard to understand execution order
- No centralized control over system lifecycle
- Difficult to enable/disable systems dynamically

**Solution:**
```cpp
class shooting : public scene {
    SystemManager m_system_mgr;
    
    void on_start() {
        // All systems registered with explicit priorities
        m_system_mgr.register_system("player", player::system, 10);
        m_system_mgr.register_system("enemy", enemy::system, 20);
        m_system_mgr.register_system("bullet", bullet::system, 30);
    }
    
    void on_update() {
        // Uniform execution
        m_system_mgr.execute(world, dt, Phase::Update);
    }
};
```

### 2. Tight Coupling Between Scene and Systems

**Problem:**
```cpp
class shooting : public scene {
    std::vector<std::variant<Texture2D, Sound>> m_to_clean;
    std::vector<systems::enemy::spawner> m_enemy_spawners;
    std::vector<systems::waves::level_cfg> m_levels;
    systems::enemy::spawner m_enemy_spawner;
    Sound m_gun_shoot_sfx;
    // ... many more system-specific details
};
```

**Issues:**
- Scene knows too much about system internals
- Violates Single Responsibility Principle
- Hard to reuse scene logic across different game modes
- Scene manages resources that belong to systems

**Solution:**
```cpp
class shooting : public scene {
    // Only scene orchestration components
    SystemManager m_system_mgr;
    ResourceManager m_resources;
    EventBus m_event_bus;
    
    // Scene-specific state only
    Camera2D m_camera;
    ginseng::database::ent_id m_player_entity;
    game_stats m_stats;
};
```

### 3. Resource Management Scattered and Manual

**Problem:**
```cpp
void on_start() {
    auto spaceship = LoadTexture("player.png");
    auto sound = LoadSound("shoot.wav");
    m_to_clean.emplace_back(spaceship);  // Manual tracking
    m_to_clean.emplace_back(sound);
}

std::unique_ptr<scene> on_exit() {
    // Manual cleanup with type checking
    for (auto res : m_to_clean) {
        std::visit([](auto& t) {
            if constexpr (std::is_same_v<decltype(t), Texture2D>) {
                UnloadTexture(t);
            } else if constexpr (std::is_same_v<decltype(t), Sound>) {
                UnloadSound(t);
            }
        }, res);
    }
}
```

**Issues:**
- Easy to forget cleanup (resource leaks)
- Not exception-safe
- Verbose and error-prone
- Type erasure with std::variant adds complexity

**Solution:**
```cpp
void on_start() {
    // ResourceManager tracks and auto-cleans
    auto spaceship = m_resources.load_texture("player.png");
    auto sound = m_resources.load_sound("shoot.wav");
    // No manual tracking needed
}

std::unique_ptr<scene> on_exit() {
    // Automatic cleanup via RAII
    // No manual code needed - ResourceManager destructor handles it
}
```

### 4. No Clear System Lifecycle Management

**Problem:**
```cpp
void shooting::on_start() {
    m_systems.emplace_back(player::update);
    m_systems.emplace_back(enemy::update);
    // No initialization phase
}

void shooting::on_update() {
    for (auto& system : m_systems) {
        system(world, dt);  // Only update, no init or cleanup
    }
}

// No on_cleanup phase
```

**Issues:**
- Systems can't do one-time initialization
- No structured cleanup phase
- Initialization code mixed with update logic
- Static variables used for initialization state

**Solution:**
```cpp
enum class Phase { Init, Update, Cleanup };

void shooting::on_start() {
    m_system_mgr.register_system("player", player::system, 10);
    // Initialize all systems
    m_system_mgr.execute(world, 0.0f, Phase::Init);
}

void shooting::on_update() {
    // Update all systems
    m_system_mgr.execute(world, dt, Phase::Update);
}

void shooting::on_exit() {
    // Cleanup all systems
    m_system_mgr.execute(world, 0.0f, Phase::Cleanup);
}
```

### 5. Direct System-to-System Dependencies

**Problem:**
```cpp
// In player system
void update(ginseng::database& db, float dt) {
    if (should_shoot) {
        // Player directly calls bullet system
        systems::bullet::shoot_bullet(db, bullet_info);
    }
}
```

**Issues:**
- Tight coupling between player and bullet systems
- Can't test player system without bullet system
- Hard to change bullet implementation
- Circular dependencies possible

**Solution:**
```cpp
// Player publishes event
void player::system(db, dt, phase, event_bus) {
    if (phase == Phase::Update && should_shoot) {
        event_bus->publish(ShootEvent{
            .position = pos,
            .rotation = rot,
            .damage = 10
        });
    }
}

// Bullet subscribes to event
void bullet::system(db, dt, phase, event_bus) {
    if (phase == Phase::Init) {
        event_bus->subscribe<ShootEvent>([&](const Event& e) {
            shoot_bullet(db, ...);
        });
    }
}
```

### 6. Global Game State via Singleton

**Problem:**
```cpp
static game* g_instance = nullptr;

game::game(config::app_info spec) {
    g_instance = this;  // Global state
}

game& game::get() { 
    return *g_instance;  // Accessed globally
}

// Used everywhere
auto& world = game::get().get_world();
```

**Issues:**
- Violates dependency inversion
- Hard to test in isolation
- Implicit dependencies
- Can't have multiple game instances

**Mitigation:**
While the global instance remains for backward compatibility, the new architecture reduces reliance on it:
```cpp
// Systems receive db explicitly
void system(ginseng::database& db, float dt, Phase phase) {
    // No need to call game::get() 
}
```

### 7. Inconsistent System Function Signatures

**Problem:**
```cpp
// Different signatures everywhere
void player::update(ginseng::database& db, float dt);
void bullet::update(ginseng::database& db, float dt, Rectangle bounds);
void enemy::update(ginseng::database& db, float dt);
void waves::update(/* custom lambda with captures */);
```

**Issues:**
- Can't store systems uniformly
- Requires special handling per system
- Hard to create generic system runners
- Inconsistent parameter passing

**Solution:**
```cpp
// Unified signature for all systems
using SystemFunc = std::function<void(ginseng::database&, float, Phase)>;

void player::system(ginseng::database& db, float dt, Phase phase);
void bullet::system(ginseng::database& db, float dt, Phase phase);
void enemy::system(ginseng::database& db, float dt, Phase phase);

// Extra parameters passed via components or events
```

### 8. Data Storage Split Between ECS and Scene

**Problem:**
```cpp
class shooting : public scene {
    // Some data in ECS
    ginseng::database::ent_id m_player_entity;
    
    // Some data in scene members
    std::vector<systems::enemy::spawner> m_enemy_spawners;
    std::vector<systems::waves::level_cfg> m_levels;
    Sound m_gun_shoot_sfx;
};
```

**Issues:**
- Unclear where data should live
- Hard to serialize/save game state
- Mixed ownership model
- Can't leverage ECS queries on scene data

**Solution:**
Move configuration and runtime data into ECS components where appropriate:
```cpp
// Level data as components
struct LevelConfig {
    std::vector<wave_cfg> waves;
    size_t current_wave;
};

// Create level entity
auto level = db.create_entity();
db.add_component(level, level_config);

// Systems operate on components
db.visit([](LevelConfig& cfg) {
    // Process level
});
```

## Summary of Solutions

### New Architecture Components

1. **SystemManager**: Centralized system registration and execution
   - Priority-based execution order
   - Enable/disable systems dynamically
   - Uniform system interface

2. **ResourceManager**: RAII-based automatic resource cleanup
   - No manual tracking
   - Exception-safe
   - Prevents resource leaks

3. **EventBus**: Publish-subscribe event system
   - Decouple systems
   - Easy to add event handlers
   - Testable in isolation

4. **Phase-based Lifecycle**: Init/Update/Cleanup phases
   - Structured initialization
   - Proper cleanup
   - Clear separation of concerns

### Migration Strategy

The solution provides backward compatibility:
- Legacy APIs maintained (`update()` functions)
- New APIs added (`system()` with phases)
- Gradual migration path
- Systems work with both approaches

### Benefits

1. **Better Organization**
   - Clear system execution order
   - Centralized lifecycle management
   - Consistent patterns

2. **Reduced Coupling**
   - Event-driven communication
   - No direct system dependencies
   - Easier testing

3. **Automatic Resource Management**
   - RAII prevents leaks
   - Exception-safe
   - Less boilerplate

4. **Maintainability**
   - Easier to understand
   - Easier to extend
   - Self-documenting code

5. **Testability**
   - Systems can be tested in isolation
   - Mock event bus for testing
   - Clear dependencies

## Next Steps

To fully adopt the new architecture:

1. Migrate remaining systems to phase-based API
2. Move more configuration into ECS components
3. Replace direct system calls with events
4. Add unit tests for new components
5. Update all scenes to use SystemManager
6. Document system dependencies and execution order

## References

- `docs/ecs_architecture.md` - Detailed architecture documentation
- `src/system_manager.cppm` - SystemManager implementation
- `src/resource_manager.cppm` - ResourceManager implementation
- `src/event_system.cppm` - EventBus implementation
