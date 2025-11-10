# ECS Architecture Refactoring - Summary

This directory contains documentation for the ECS (Entity Component System) architecture refactoring that addresses game flow inconsistencies.

## Quick Links

- **[ECS Architecture Guide](ecs_architecture.md)** - Complete architecture overview
- **[Game Flow Inconsistencies](game_flow_inconsistencies.md)** - Detailed problem analysis
- **[Migration Guide](migration_guide.md)** - Step-by-step migration instructions

## What Was Done

### Problem Statement

The original request was to:
> "identify the inconsistencies in the game flow and suggest a way to separate my game in different pieces by using the given ecs library (or can also be entt)"

### Solution Overview

We identified **8 major inconsistencies** in the game architecture and implemented a comprehensive solution using the ginseng ECS library with three core components:

1. **SystemManager** - Centralized system registration and execution
2. **ResourceManager** - RAII-based automatic resource cleanup  
3. **EventBus** - Publish-subscribe event system for decoupled communication

### Key Improvements

#### Before
```cpp
class shooting : public scene {
    std::vector<std::function<void(ginseng::database&, float)>> m_systems;
    std::vector<std::variant<Texture2D, Sound>> m_to_clean;
    // Manual resource tracking, mixed execution patterns
};
```

#### After
```cpp
class shooting : public scene {
    systems::SystemManager m_system_mgr;    // Unified system execution
    systems::ResourceManager m_resources;    // Auto resource cleanup
    systems::EventBus m_event_bus;          // Decoupled communication
};
```

## Major Inconsistencies Identified

1. **Mixed System Organization** - Inconsistent execution patterns
2. **Tight Scene-System Coupling** - Scene knows too much about systems
3. **Manual Resource Management** - Error-prone cleanup code
4. **No Lifecycle Management** - Missing init/cleanup phases
5. **Direct System Dependencies** - Systems calling each other directly
6. **Global State Singleton** - Over-reliance on game::get()
7. **Inconsistent Function Signatures** - Each system different
8. **Split Data Storage** - Data scattered between ECS and scene

## Files Added

### Core Architecture (`/src`)
- `system_manager.cppm` - SystemManager implementation
- `resource_manager.cppm` - ResourceManager implementation
- `event_system.cppm` - EventBus implementation

### Documentation (`/docs`)
- `ecs_architecture.md` - Architecture guide (7.5KB)
- `game_flow_inconsistencies.md` - Problem analysis (10KB)
- `migration_guide.md` - Migration instructions (11KB)

### Updated Systems (`/src/systems`)
- `player.cppm/cpp` - Phase-based system with event support
- `bullet.cppm/cpp` - Event-driven bullet spawning
- `enemy.cppm/cpp` - Phase-based lifecycle

### Updated Scenes (`/src/scenes`)
- `shooting.cppm/cpp` - Uses new managers with backward compatibility

## How To Use

### For Understanding the Architecture
1. Start with [ecs_architecture.md](ecs_architecture.md) for the big picture
2. Read [game_flow_inconsistencies.md](game_flow_inconsistencies.md) to understand the problems
3. Review code examples in the updated systems

### For Migrating Your Code
1. Follow [migration_guide.md](migration_guide.md) step-by-step
2. Start with scenes, then systems
3. Test incrementally
4. Maintain backward compatibility during migration

## Example: Player Shooting

### Before (Tight Coupling)
```cpp
// Player system directly calls bullet system
void player::update(ginseng::database& db, float dt) {
    if (should_shoot) {
        systems::bullet::shoot_bullet(db, bullet_info);
    }
}
```

### After (Event-Driven)
```cpp
// Player publishes event
void player::system(db, dt, phase, event_bus) {
    if (should_shoot) {
        event_bus->publish(ShootEvent{...});
    }
}

// Bullet subscribes to event (in Init phase)
void bullet::system(db, dt, phase, event_bus) {
    if (phase == Phase::Init) {
        event_bus->subscribe<ShootEvent>([&](const Event& e) {
            shoot_bullet(db, ...);
        });
    }
}
```

## Benefits

### Code Quality
- ✅ Clear separation of concerns
- ✅ Consistent system patterns
- ✅ Self-documenting execution order
- ✅ Reduced coupling between systems

### Resource Management
- ✅ Automatic cleanup (RAII)
- ✅ Exception-safe
- ✅ No manual tracking needed
- ✅ Prevents resource leaks

### Testing & Maintenance
- ✅ Systems testable in isolation
- ✅ Easy to add new systems
- ✅ Clear dependencies via events
- ✅ Better error messages

### Developer Experience
- ✅ Easier to understand code flow
- ✅ Safer refactoring
- ✅ Less boilerplate
- ✅ Clear upgrade path

## Current State

### ✅ Completed
- Core architecture components (SystemManager, ResourceManager, EventBus)
- Phase-based system lifecycle (Init/Update/Cleanup)
- Event-driven communication infrastructure
- Backward compatibility with legacy APIs
- Comprehensive documentation
- Example implementations in player/bullet/enemy systems

### 🔄 In Progress
- Full migration of shooting scene to new architecture
- Moving configuration data into ECS components
- Removing all direct system-to-system calls

### 📋 Future Work
- Migrate remaining scenes (start_screen, gameover)
- Add unit tests for new components
- Performance profiling per-system
- Scene graph for hierarchical management
- Component serialization for save/load

## Architecture Diagram

```
┌─────────────────────────────────────────────────────────┐
│                         Scene                            │
│  ┌────────────────┐ ┌─────────────────┐ ┌────────────┐ │
│  │ SystemManager  │ │ResourceManager  │ │  EventBus  │ │
│  └────────┬───────┘ └─────────────────┘ └──────┬─────┘ │
│           │                                     │        │
└───────────┼─────────────────────────────────────┼────────┘
            │                                     │
            ▼                                     ▼
   ┌────────────────┐                   ┌─────────────────┐
   │  System Init   │◄─────Events──────►│ Event Handlers  │
   │  System Update │                   │  (Subscribe in  │
   │  System Cleanup│                   │   Init phase)   │
   └────────┬───────┘                   └─────────────────┘
            │
            ▼
   ┌────────────────┐
   │  ECS Database  │
   │   (ginseng)    │
   │                │
   │  - Entities    │
   │  - Components  │
   └────────────────┘
```

## Testing

The new architecture enables better testing:

```cpp
TEST(PlayerSystem, FiresShootEvent) {
    ginseng::database db;
    systems::EventBus bus;
    
    bool event_received = false;
    bus.subscribe<ShootEvent>([&](const Event&) {
        event_received = true;
    });
    
    player::system(db, dt, Phase::Init, &bus);
    // Simulate input...
    player::system(db, dt, Phase::Update, &bus);
    
    ASSERT_TRUE(event_received);
}
```

## Questions?

Refer to:
- **Architecture questions**: [ecs_architecture.md](ecs_architecture.md)
- **Migration help**: [migration_guide.md](migration_guide.md)
- **Problem context**: [game_flow_inconsistencies.md](game_flow_inconsistencies.md)

## Contributing

When adding new systems:
1. Follow the phase-based pattern (Init/Update/Cleanup)
2. Use events for inter-system communication
3. Register with appropriate priority
4. Document any special dependencies
5. Add tests for system behavior

## License

Same as the main project.
