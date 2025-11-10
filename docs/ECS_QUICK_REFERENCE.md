# ECS Quick Reference Card

> **Quick guide to the new ECS architecture patterns**

## System Template

```cpp
// my_system.cppm
export module systems.my_system;
import system_manager;
import event_system;
import ginseng;

export namespace systems::my_system {
    void system(ginseng::database& db, float dt, 
                systems::Phase phase, 
                systems::EventBus* bus = nullptr);
}
```

```cpp
// my_system.cpp
module systems.my_system;

void system(ginseng::database& db, float dt, 
            systems::Phase phase, systems::EventBus* bus) {
    
    if (phase == systems::Phase::Init) {
        // One-time initialization
        if (bus) {
            // Subscribe to events
            bus->subscribe<SomeEvent>([&](const systems::Event& e) {
                // Handle event
            });
        }
        return;
    }
    
    if (phase == systems::Phase::Cleanup) {
        // Cleanup resources
        return;
    }
    
    // Phase::Update - regular game logic
    db.visit([dt](MyComponent& comp) {
        // Update components
    });
}
```

## Scene Template

```cpp
// my_scene.cppm
export module scenes.my_scene;
import system_manager;
import resource_manager;
import event_system;
import game;

export namespace scenes {
    class my_scene : public scene {
        systems::SystemManager m_system_mgr;
        systems::ResourceManager m_resources;
        systems::EventBus m_event_bus;
        
    public:
        void on_start() override;
        void on_update() override;
        void on_render() override;
        std::unique_ptr<scene> on_exit() override;
    };
}
```

```cpp
// my_scene.cpp
module scenes.my_scene;

void my_scene::on_start() {
    // Register systems with priorities
    m_system_mgr.register_system("input", 
        [this](auto& db, float dt, auto phase) {
            input::system(db, dt, phase, &m_event_bus);
        }, 10);
    
    m_system_mgr.register_system("physics",
        [this](auto& db, float dt, auto phase) {
            physics::system(db, dt, phase);
        }, 20);
    
    // Initialize all systems
    auto& world = game::get().get_world();
    m_system_mgr.execute(world, 0.0f, systems::Phase::Init);
}

void my_scene::on_update() {
    auto& world = game::get().get_world();
    float dt = GetFrameTime();
    m_system_mgr.execute(world, dt, systems::Phase::Update);
}

void my_scene::on_exit() {
    auto& world = game::get().get_world();
    m_system_mgr.execute(world, 0.0f, systems::Phase::Cleanup);
    // Resources auto-cleaned by destructors
    return nullptr;
}
```

## Common Patterns

### Loading Resources

```cpp
// ✅ Good - automatic cleanup
void on_start() {
    auto texture = m_resources.load_texture("sprite.png");
    auto sound = m_resources.load_sound("effect.wav");
    
    db.add_component(entity, texture);
    db.add_component(entity, sound);
}

// ❌ Bad - manual tracking
void on_start() {
    auto texture = LoadTexture("sprite.png");
    m_to_clean.push_back(texture);  // Manual tracking
}
```

### Publishing Events

```cpp
// In any system
if (condition) {
    event_bus->publish(MyEvent{
        .value1 = 42,
        .value2 = {100, 200}
    });
}
```

### Subscribing to Events

```cpp
// In Init phase only!
if (phase == systems::Phase::Init) {
    event_bus->subscribe<MyEvent>([&](const systems::Event& e) {
        auto& evt = static_cast<const MyEvent&>(e);
        // Handle event
    });
}
```

### System Priority Examples

```cpp
// Lower number = runs first
m_system_mgr.register_system("input", input::system, 10);      // First
m_system_mgr.register_system("player", player::system, 20);    
m_system_mgr.register_system("enemy", enemy::system, 30);      
m_system_mgr.register_system("physics", physics::system, 40);  
m_system_mgr.register_system("collision", collision::system, 50);
m_system_mgr.register_system("audio", audio::system, 90);      
m_system_mgr.register_system("render", render::system, 100);   // Last
```

### Passing Extra Parameters

```cpp
// Capture in lambda
m_system_mgr.register_system("camera",
    [this](auto& db, float dt, auto phase) {
        if (phase == systems::Phase::Update) {
            auto bounds = get_camera_bounds(m_camera);
            camera::system(db, dt, bounds);
        }
    }, 25);
```

## Event Definition

```cpp
// In event_system.cppm
struct MyCustomEvent : Event {
    Vector2 position;
    int damage;
    float radius;
};
```

## Component Definition

```cpp
// In components/my_component.cppm
export module components.my_component;
import raylib;

export namespace components {
    // Pure data - no methods!
    struct MyComponent {
        Vector2 position;
        float speed;
        int health;
    };
}
```

## Checklist for New Systems

- [ ] Create `systems/my_system.cppm` module
- [ ] Implement `system(db, dt, phase, bus)` function
- [ ] Handle Init, Update, Cleanup phases
- [ ] Subscribe to events in Init phase (if needed)
- [ ] Register in scene's `on_start()`
- [ ] Set appropriate priority
- [ ] Document system dependencies

## Checklist for New Scenes

- [ ] Add `SystemManager m_system_mgr`
- [ ] Add `ResourceManager m_resources`
- [ ] Add `EventBus m_event_bus`
- [ ] Register all systems in `on_start()`
- [ ] Call `execute(db, 0, Phase::Init)` in `on_start()`
- [ ] Call `execute(db, dt, Phase::Update)` in `on_update()`
- [ ] Call `execute(db, 0, Phase::Cleanup)` in `on_exit()`
- [ ] Use `m_resources.load_*()` for all resources

## Common Mistakes

### ❌ Subscribing in Update Phase

```cpp
if (phase == systems::Phase::Update) {
    event_bus->subscribe<MyEvent>([](auto&) {});  // WRONG!
}
```

**Why**: Subscribes every frame, memory leak

### ✅ Subscribe Once in Init

```cpp
if (phase == systems::Phase::Init) {
    event_bus->subscribe<MyEvent>([](auto&) {});  // Correct
}
```

### ❌ Manual Resource Loading

```cpp
auto tex = LoadTexture("sprite.png");  // WRONG!
```

**Why**: Forget to unload → resource leak

### ✅ ResourceManager Loading

```cpp
auto tex = m_resources.load_texture("sprite.png");  // Correct
```

### ❌ Direct System Calls

```cpp
// In system A
systems::system_b::do_something(db, param);  // WRONG!
```

**Why**: Tight coupling, hard to test

### ✅ Event-Driven Communication

```cpp
// In system A
event_bus->publish(SomethingEvent{param});  // Correct

// In system B (Init phase)
event_bus->subscribe<SomethingEvent>([&](auto& e) {
    do_something(db, e.param);
});
```

## Phase Usage Rules

| Phase   | Use For                                    | Don't Use For           |
|---------|-------------------------------------------|-------------------------|
| Init    | Subscribe to events, load static data     | Game logic, spawning    |
| Update  | Game logic, component updates             | Event subscriptions     |
| Cleanup | Unload resources, save state              | Game logic              |

## System Communication Patterns

### Pattern 1: Request-Response

```cpp
// System A publishes request
event_bus->publish(RequestEvent{.entity_id = id});

// System B handles and responds
event_bus->subscribe<RequestEvent>([&](auto& e) {
    auto result = process(e.entity_id);
    event_bus->publish(ResponseEvent{.result = result});
});

// System A handles response
event_bus->subscribe<ResponseEvent>([&](auto& e) {
    handle_result(e.result);
});
```

### Pattern 2: Broadcast Notification

```cpp
// Any system can publish
if (enemy_died) {
    event_bus->publish(EnemyDiedEvent{
        .position = enemy_pos,
        .score = 100
    });
}

// Multiple systems can subscribe
// Score system
event_bus->subscribe<EnemyDiedEvent>([&](auto& e) {
    score += e.score;
});

// Particle system
event_bus->subscribe<EnemyDiedEvent>([&](auto& e) {
    spawn_explosion(e.position);
});

// Audio system
event_bus->subscribe<EnemyDiedEvent>([&](auto&) {
    play_sound("explosion.wav");
});
```

## Testing Template

```cpp
TEST(MySystem, DoesExpectedBehavior) {
    // Setup
    ginseng::database db;
    systems::EventBus bus;
    
    auto entity = db.create_entity();
    db.add_component(entity, MyComponent{});
    
    // Initialize
    my_system::system(db, 0.0f, systems::Phase::Init, &bus);
    
    // Execute
    my_system::system(db, 0.016f, systems::Phase::Update, &bus);
    
    // Verify
    auto& comp = db.get_component<MyComponent>(entity);
    ASSERT_EQ(comp.value, expected_value);
}
```

## Performance Tips

1. **System Priority Matters**
   - Physics before collision
   - Input before player logic
   - Rendering last

2. **Minimize Event Publishing**
   - Only publish when state changes
   - Not every frame if unchanged

3. **Use Components Wisely**
   - Group related data
   - Avoid deep nesting
   - Cache frequently accessed components

4. **ResourceManager**
   - Load resources once (in Init)
   - Share textures across entities
   - Don't reload every frame

## Getting Help

- Architecture: `docs/ecs_architecture.md`
- Migration: `docs/migration_guide.md`
- Problems: `docs/game_flow_inconsistencies.md`
- Examples: `src/scenes/shooting.cpp`, `src/systems/player.cpp`
