# ECS Architecture Documentation

## Overview

This document describes the Entity Component System (ECS) architecture used in the tower defense game, built on top of the `ginseng` ECS library.

## Architecture Principles

### 1. Separation of Concerns

The game is now structured into distinct, loosely-coupled layers:

- **Components**: Pure data structures (no behavior)
- **Systems**: Logic that operates on components
- **Managers**: Orchestrate system execution and resource lifecycle
- **Events**: Decouple inter-system communication

### 2. System Organization

All systems follow a consistent pattern with three lifecycle phases:

#### System Phases

```cpp
enum class Phase {
    Init,      // One-time initialization (e.g., subscribe to events)
    Update,    // Per-frame update logic
    Cleanup    // Cleanup/shutdown (e.g., unload resources)
};
```

#### System Signature

```cpp
void system(ginseng::database& db, float dt, Phase phase);
```

Every system implements this unified interface, allowing the SystemManager to execute them uniformly.

## Core Components

### SystemManager

Centralizes system registration and execution:

```cpp
SystemManager mgr;
mgr.register_system("player", player::system, 10);    // Priority 10 (runs first)
mgr.register_system("enemy", enemy::system, 20);      // Priority 20 (runs second)
mgr.register_system("bullet", bullet::system, 30);    // Priority 30 (runs last)

// Execute all systems
mgr.execute(db, dt, Phase::Update);
```

**Benefits:**
- Clear system execution order via priorities
- Easy to enable/disable systems
- Single point of control for system lifecycle

### ResourceManager

Automatic resource cleanup using RAII:

```cpp
ResourceManager resources;
auto texture = resources.load_texture("player.png");
auto sound = resources.load_sound("shoot.wav");
// Resources automatically cleaned up when manager is destroyed
```

**Benefits:**
- No manual cleanup needed
- Prevents resource leaks
- Exception-safe resource management

### EventBus

Decouples inter-system communication:

```cpp
// System A: Publish event
event_bus.publish(ShootEvent{
    .position = player_pos,
    .rotation = player_rot,
    .damage = 10
});

// System B: Subscribe to event (typically in Init phase)
event_bus.subscribe<ShootEvent>([&db](const Event& event) {
    auto& shoot = static_cast<const ShootEvent&>(event);
    // Create bullet entity...
});
```

**Benefits:**
- Systems don't need to know about each other
- Easy to add new event handlers
- Testable in isolation

## System Dependencies

### Before (Tightly Coupled)

```cpp
// Player system directly calls bullet system
systems::bullet::shoot_bullet(db, bullet_info);
```

Problems:
- Player system depends on bullet system
- Hard to test player without bullet
- Changes to bullet affect player

### After (Event-Driven)

```cpp
// Player publishes event
event_bus.publish(ShootEvent{...});

// Bullet subscribes to event (in Init phase)
event_bus.subscribe<ShootEvent>([&](const Event& e) {
    shoot_bullet(db, ...);
});
```

Benefits:
- Player and bullet are decoupled
- Can test player without bullet system
- Easy to add more shoot event handlers

## Scene Structure

### Before (Monolithic)

```cpp
class shooting : public scene {
    std::vector<std::function<void(ginseng::database&, float)>> m_systems;
    std::vector<std::variant<Texture2D, Sound>> m_to_clean;
    std::vector<systems::enemy::spawner> m_enemy_spawners;
    // ... many more members
};
```

Problems:
- Scene manages too many concerns
- Hard to understand what's happening
- Difficult to reuse across scenes

### After (Focused)

```cpp
class shooting : public scene {
    SystemManager system_mgr;
    ResourceManager resources;
    EventBus event_bus;
    // Scene-specific configuration only
};
```

Benefits:
- Clear responsibilities
- Reusable managers
- Easy to understand

## Migration Guide

### Adding a New System

1. **Create system module** (`systems/my_system.cppm`):

```cpp
export module systems.my_system;
import system_manager;
import ginseng;

export namespace systems::my_system {
    void system(ginseng::database& db, float dt, Phase phase) {
        if (phase == Phase::Init) {
            // Initialize (subscribe to events, etc.)
        } else if (phase == Phase::Update) {
            // Update logic
        } else if (phase == Phase::Cleanup) {
            // Cleanup
        }
    }
}
```

2. **Register in scene**:

```cpp
void shooting::on_start() {
    system_mgr.register_system("my_system", 
        [](auto& db, float dt, auto phase) {
            systems::my_system::system(db, dt, phase);
        }, 
        priority);
}
```

### Converting Existing Systems

1. **Keep legacy API for backward compatibility**:

```cpp
void update(ginseng::database& db, float dt);  // Old API
void system(ginseng::database& db, float dt, Phase phase);  // New API
```

2. **Implement new API**:

```cpp
void system(ginseng::database& db, float dt, Phase phase) {
    if (phase == Phase::Update) {
        update(db, dt);  // Call legacy function
    }
}
```

3. **Gradually migrate call sites** from `update()` to `system()`.

## Best Practices

### 1. Keep Components Pure Data

❌ Bad:
```cpp
struct Player {
    Vector2 position;
    void update(float dt) { /* logic */ }  // No methods!
};
```

✅ Good:
```cpp
struct Player {
    Vector2 position;
};

// Logic goes in system
void player_system(db, dt, phase) { /* logic */ }
```

### 2. Use Events for Inter-System Communication

❌ Bad:
```cpp
// System A directly calls System B
systems::other::do_something(db, param);
```

✅ Good:
```cpp
// System A publishes event
event_bus.publish(SomethingEvent{param});

// System B subscribes in Init phase
event_bus.subscribe<SomethingEvent>([&](const Event& e) {
    do_something(db, param);
});
```

### 3. Initialize Systems in Init Phase

❌ Bad:
```cpp
void system(db, dt, phase) {
    if (phase == Phase::Update) {
        static bool initialized = false;
        if (!initialized) {
            // Init code
            initialized = true;
        }
        // Update code
    }
}
```

✅ Good:
```cpp
void system(db, dt, phase) {
    if (phase == Phase::Init) {
        // Init code
    } else if (phase == Phase::Update) {
        // Update code
    }
}
```

### 4. Use SystemManager for System Execution

❌ Bad:
```cpp
void on_update() {
    player::update(db, dt);
    enemy::update(db, dt);
    bullet::update(db, dt);
}
```

✅ Good:
```cpp
void on_start() {
    system_mgr.register_system("player", player::system, 10);
    system_mgr.register_system("enemy", enemy::system, 20);
    system_mgr.register_system("bullet", bullet::system, 30);
}

void on_update() {
    system_mgr.execute(db, dt, Phase::Update);
}
```

## Testing

The new architecture makes testing much easier:

```cpp
// Test player system in isolation
TEST(PlayerSystem, ShootsWhenSpacePressed) {
    ginseng::database db;
    EventBus bus;
    
    // Setup event capture
    bool shoot_event_fired = false;
    bus.subscribe<ShootEvent>([&](const Event&) {
        shoot_event_fired = true;
    });
    
    // Run system
    player::system(db, dt, Phase::Init, &bus);
    player::system(db, dt, Phase::Update, &bus);
    
    ASSERT_TRUE(shoot_event_fired);
}
```

## Future Improvements

1. **Scene Graph**: Hierarchical scene management
2. **Component Serialization**: Save/load game state
3. **System Profiling**: Performance monitoring per-system
4. **Hot Reloading**: Reload systems without restarting
5. **Thread Pool**: Parallel system execution
