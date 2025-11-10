# Migration Guide: Moving to New ECS Architecture

This guide helps you migrate existing code to use the new SystemManager, ResourceManager, and EventBus architecture.

## Quick Start

### 1. Update Your Scene

**Before:**
```cpp
// shooting.cppm
class shooting : public scene {
    std::vector<std::function<void(ginseng::database&, float)>> m_systems;
    std::vector<std::variant<Texture2D, Sound>> m_to_clean;
    // ...
};
```

**After:**
```cpp
// shooting.cppm
import system_manager;
import resource_manager;
import event_system;

class shooting : public scene {
    systems::SystemManager m_system_mgr;
    systems::ResourceManager m_resources;
    systems::EventBus m_event_bus;
    // ...
};
```

### 2. Register Systems in on_start()

**Before:**
```cpp
void shooting::on_start() {
    m_systems.emplace_back(player::update);
    m_systems.emplace_back(enemy::update);
}
```

**After:**
```cpp
void shooting::on_start() {
    // Register with priorities (lower = earlier)
    m_system_mgr.register_system("player", 
        [this](auto& db, float dt, auto phase) {
            player::system(db, dt, phase, &m_event_bus);
        }, 10);
    
    m_system_mgr.register_system("enemy",
        [](auto& db, float dt, auto phase) {
            enemy::system(db, dt, phase);
        }, 20);
    
    // Initialize all systems
    m_system_mgr.execute(world, 0.0f, systems::Phase::Init);
}
```

### 3. Update on_update()

**Before:**
```cpp
void shooting::on_update() {
    float dt = GetFrameTime();
    auto& world = game::get().get_world();
    
    for (auto& system : m_systems) {
        system(world, dt);
    }
    
    bullet::update(world, dt, bounds);
}
```

**After:**
```cpp
void shooting::on_update() {
    float dt = GetFrameTime();
    auto& world = game::get().get_world();
    
    // Execute all registered systems
    m_system_mgr.execute(world, dt, systems::Phase::Update);
}
```

### 4. Update on_exit()

**Before:**
```cpp
std::unique_ptr<scene> on_exit() {
    for (auto res : m_to_clean) {
        std::visit([](auto& t) {
            if constexpr (std::is_same_v<decltype(t), Texture2D>) {
                UnloadTexture(t);
            } else if constexpr (std::is_same_v<decltype(t), Sound>) {
                UnloadSound(t);
            }
        }, res);
    }
    return std::make_unique<scenes::gameover>(m_stats);
}
```

**After:**
```cpp
std::unique_ptr<scene> on_exit() {
    auto& world = game::get().get_world();
    
    // Cleanup all systems
    m_system_mgr.execute(world, 0.0f, systems::Phase::Cleanup);
    
    // Resources auto-cleaned by ResourceManager destructor
    
    return std::make_unique<scenes::gameover>(m_stats);
}
```

## Migrating Systems

### Step 1: Add Phase-Based Function

**Before:**
```cpp
// player.cppm
export namespace systems::player {
    void update(ginseng::database& db, float dt);
    void draw(ginseng::database& db);
}
```

**After:**
```cpp
// player.cppm
import system_manager;
import event_system;

export namespace systems::player {
    // New unified system function
    void system(ginseng::database& db, float dt, systems::Phase phase, 
                systems::EventBus* event_bus = nullptr);
    
    // Keep legacy for backward compatibility
    void update(ginseng::database& db, float dt);
    void draw(ginseng::database& db);
}
```

### Step 2: Implement Phase Handling

**player.cpp:**
```cpp
void system(ginseng::database& db, float dt, systems::Phase phase, 
            systems::EventBus* event_bus) {
    if (phase == systems::Phase::Init) {
        // One-time initialization
        if (event_bus) {
            // Subscribe to events
        }
        return;
    }
    
    if (phase == systems::Phase::Cleanup) {
        // Cleanup code
        return;
    }
    
    // Phase::Update
    update_impl(db, dt, event_bus);
}

// Internal implementation
static void update_impl(ginseng::database& db, float dt, 
                       systems::EventBus* event_bus) {
    // Existing update logic...
}

// Legacy function calls new implementation
void update(ginseng::database& db, float dt) {
    update_impl(db, dt, nullptr);
}
```

## Resource Management

### Loading Resources

**Before:**
```cpp
void on_start() {
    auto texture = LoadTexture("player.png");
    auto sound = LoadSound("shoot.wav");
    
    m_to_clean.emplace_back(texture);
    m_to_clean.emplace_back(sound);
    
    world.add_component(entity, texture);
    world.add_component(entity, sound);
}
```

**After:**
```cpp
void on_start() {
    // ResourceManager tracks and auto-cleans
    auto texture = m_resources.load_texture("player.png");
    auto sound = m_resources.load_sound("shoot.wav");
    
    // No manual tracking needed
    world.add_component(entity, texture);
    world.add_component(entity, sound);
}
```

### Multiple Resources

```cpp
void on_start() {
    // Load multiple resources
    for (const auto& enemy_cfg : enemy_configs) {
        auto texture = m_resources.load_texture(enemy_cfg.texture_path);
        auto entity = db.create_entity();
        db.add_component(entity, texture);
    }
    
    // All textures auto-cleaned when scene exits
}
```

## Event-Based Communication

### Replacing Direct Calls

**Before (Tight Coupling):**
```cpp
// In player system
void update(ginseng::database& db, float dt) {
    if (should_shoot) {
        // Direct call to bullet system
        systems::bullet::shoot_bullet(db, bullet_info);
    }
}
```

**After (Event-Driven):**
```cpp
// In player system
void system(db, dt, phase, event_bus) {
    if (phase == Phase::Update && should_shoot) {
        // Publish event instead
        event_bus->publish(systems::ShootEvent{
            .position = player_pos,
            .velocity = player_vel,
            .rotation = player_rot,
            .damage = 10
        });
    }
}
```

### Subscribing to Events

**In bullet system:**
```cpp
void system(db, dt, phase, event_bus) {
    if (phase == Phase::Init) {
        // Subscribe once during initialization
        event_bus->subscribe<systems::ShootEvent>([&db](const systems::Event& e) {
            auto& shoot = static_cast<const systems::ShootEvent&>(e);
            shoot_bullet(db, components::bullet{
                .position = shoot.position,
                .velocity = shoot.velocity,
                .rotation = shoot.rotation,
                .damage = static_cast<uint8_t>(shoot.damage),
                .radius = 4.5f
            });
        });
    }
    
    if (phase == Phase::Update) {
        // Regular update logic
        update(db, dt);
    }
}
```

### Creating New Events

**1. Define event in event_system.cppm:**
```cpp
struct MyCustomEvent : Event {
    Vector2 position;
    int value;
};
```

**2. Publish from any system:**
```cpp
event_bus->publish(MyCustomEvent{
    .position = {100, 200},
    .value = 42
});
```

**3. Subscribe in Init phase:**
```cpp
if (phase == Phase::Init) {
    event_bus->subscribe<MyCustomEvent>([](const Event& e) {
        auto& custom = static_cast<const MyCustomEvent&>(e);
        // Handle event...
    });
}
```

## System Priorities

Control execution order with priorities:

```cpp
// Lower priority = runs first
m_system_mgr.register_system("input", input::system, 10);      // First
m_system_mgr.register_system("player", player::system, 20);    // Second
m_system_mgr.register_system("physics", physics::system, 30);  // Third
m_system_mgr.register_system("collision", collision::system, 40); // Fourth
m_system_mgr.register_system("render", render::system, 100);   // Last
```

## Common Patterns

### Pattern 1: System with External Dependencies

```cpp
void shooting::on_start() {
    // Capture camera in lambda
    m_system_mgr.register_system("bullet",
        [this](auto& db, float dt, auto phase) {
            if (phase == Phase::Update) {
                auto cam = get_camera_bounds(m_camera);
                bullet::update(db, dt, cam);
            }
        }, 30);
}
```

### Pattern 2: Conditional System Execution

```cpp
void shooting::on_update() {
    // Disable system temporarily
    if (game_paused) {
        m_system_mgr.set_enabled("enemy", false);
    } else {
        m_system_mgr.set_enabled("enemy", true);
    }
    
    m_system_mgr.execute(world, dt, systems::Phase::Update);
}
```

### Pattern 3: System-Specific Cleanup

```cpp
void my_system::system(db, dt, phase) {
    static std::vector<Texture2D> cached_textures;
    
    if (phase == Phase::Init) {
        // Load textures
        cached_textures.push_back(LoadTexture("..."));
    }
    
    if (phase == Phase::Cleanup) {
        // Clean up
        for (auto& tex : cached_textures) {
            UnloadTexture(tex);
        }
        cached_textures.clear();
    }
}
```

## Testing

The new architecture makes testing easier:

```cpp
TEST(PlayerSystem, ShootsOnSpacePress) {
    ginseng::database db;
    systems::EventBus bus;
    
    // Setup
    bool shot_fired = false;
    bus.subscribe<systems::ShootEvent>([&](const systems::Event&) {
        shot_fired = true;
    });
    
    // Initialize
    player::system(db, 0.0f, systems::Phase::Init, &bus);
    
    // Simulate space press and update
    // ... (mock input)
    player::system(db, 0.016f, systems::Phase::Update, &bus);
    
    // Verify
    ASSERT_TRUE(shot_fired);
}
```

## Troubleshooting

### System Not Executing

Check:
1. Is it registered? `m_system_mgr.register_system(...)`
2. Is it enabled? `m_system_mgr.set_enabled("name", true)`
3. Is Init called? `execute(db, 0, Phase::Init)` in on_start()

### Events Not Received

Check:
1. Subscribe in Init phase, not Update
2. Event type matches exactly
3. Publish after subscription

### Resources Not Cleaned

Check:
1. Using `m_resources.load_*()` not `Load*()`
2. ResourceManager lives long enough
3. Not copying ResourceManager (use move if needed)

## Backward Compatibility

During migration, both old and new APIs work:

```cpp
// Old API still works
void on_update() {
    player::update(db, dt);
    enemy::update(db, dt);
}

// New API preferred
void on_update() {
    m_system_mgr.execute(db, dt, Phase::Update);
}
```

Migrate gradually, one system at a time.

## Best Practices

1. **Always use ResourceManager for raylib resources**
   - Prevents leaks
   - Exception-safe
   - Less code

2. **Use events for inter-system communication**
   - Loose coupling
   - Easy to test
   - Clear dependencies

3. **Set explicit system priorities**
   - Documents execution order
   - Avoids subtle bugs
   - Self-documenting

4. **Keep components as pure data**
   - No methods
   - Easy to serialize
   - ECS best practice

5. **Initialize in Init phase, not Update**
   - Clear separation
   - Runs once
   - No static state hacks

## Further Reading

- `docs/ecs_architecture.md` - Complete architecture guide
- `docs/game_flow_inconsistencies.md` - Problem analysis
- Examples in `src/scenes/shooting.cpp`
