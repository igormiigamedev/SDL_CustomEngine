#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "../Object/GameObject.h"

// Forward declaration to avoid circular dependencies
class RigidBody;

struct CollisionEvent {
    RigidBody* bodyA;
    RigidBody* bodyB;

    CollisionEvent(RigidBody* a, RigidBody* b) : bodyA(a), bodyB(b) {}
};

class EventDispatcher {
public:
    using CollisionCallback = std::function<void(RigidBody*)>;
    using CollisionFilter = std::function<bool(const CollisionEvent&)>;

    static EventDispatcher* GetInstance();

    // Register a callback for collision events
    void RegisterCollisionCallback(std::weak_ptr<GameObject> owner, CollisionCallback callback, CollisionFilter filter);

    // Dispatch a collision event to all registered callbacks
    void DispatchCollisionEvent(const CollisionEvent& event);

    // Unregister all callbacks associated with a specific owner
    void UnregisterCallback(const GameObject* owner);

private:
    struct CallbackEntry {
        std::weak_ptr<GameObject> owner;
        CollisionCallback callback;
        CollisionFilter filter;
    };

    std::vector<CallbackEntry> m_CollisionCallbacks;

    // Clean up invalid (expired) entries from the callback list
    void CleanupInvalidCallbacks();
};
