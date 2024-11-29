#pragma once

// Early declaration to avoid circular dependency
class RigidBody;

struct CollisionEvent {
    RigidBody* bodyA;
    RigidBody* bodyB;
    /*TileMap* tileMap;*/  // If want to have a collision event with Map (optional, it can be nullptr)

    // Constructors for different collision cases
    /*CollisionEvent(RigidBody* a, RigidBody* b) : bodyA(a), bodyB(b), tileMap(nullptr) {}
    CollisionEvent(RigidBody* a, TileMap* map) : bodyA(a), bodyB(nullptr), tileMap(map) {}*/

    CollisionEvent(RigidBody* a, RigidBody* b) : bodyA(a), bodyB(b) {}
};

class EventDispatcher {
public:
    using CollisionCallback = std::function<void(const CollisionEvent&)>;

    static EventDispatcher* GetInstance() {
        static EventDispatcher instance;
        return &instance;
    }

    void RegisterCollisionCallback(CollisionCallback callback) {
        m_CollisionCallbacks.push_back(callback);
    }

    void DispatchCollisionEvent(const CollisionEvent& event) {
        for (auto& callback : m_CollisionCallbacks) {
            callback(event);
        }
    }

private:
    std::vector<CollisionCallback> m_CollisionCallbacks;

    // Private constructor to ensure it is singleton
    EventDispatcher() = default;

    // Disable copying and attribution
    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;
};

