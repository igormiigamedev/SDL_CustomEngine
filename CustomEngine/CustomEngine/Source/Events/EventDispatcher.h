#pragma once
#include <functional>
#include <vector>
#include "../Object/GameObject.h"
// Early declaration to avoid circular dependency
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

    void RegisterCollisionCallback(std::weak_ptr<GameObject> owner, CollisionCallback callback, CollisionFilter filter);
    void DispatchCollisionEvent(const CollisionEvent& event);

    void UnregisterCallback(const std::weak_ptr<GameObject>& owner);

private:
    struct CallbackEntry {
        std::weak_ptr<GameObject> owner;
        CollisionCallback callback;
        CollisionFilter filter;
    };

    std::vector<CallbackEntry> m_CollisionCallbacks;

    EventDispatcher() = default;
};


