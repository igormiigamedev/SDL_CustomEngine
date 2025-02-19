#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "../Object/GameObject.h"
#include "../../ECollisionDirection.h"

// Forward declaration to avoid circular dependencies
class RigidBody;

struct CollisionEvent {
    std::weak_ptr<GameObject> objectA;
    std::weak_ptr<GameObject> objectB;
    CollisionDirection collisionDirection;

    CollisionEvent(const std::weak_ptr<GameObject>& a, const std::weak_ptr<GameObject>& b, CollisionDirection direction)
        : objectA(a), objectB(b), collisionDirection(direction) {}
};

class EventDispatcher {
public:
    using CollisionCallback = std::function<void(GameObject*, CollisionDirection)>;
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
