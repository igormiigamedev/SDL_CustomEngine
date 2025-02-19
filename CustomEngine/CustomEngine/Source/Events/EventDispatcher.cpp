#include "EventDispatcher.h"
#include "../Physics/RigidBody.h"
#include <iostream>
#include <algorithm>

EventDispatcher* EventDispatcher::GetInstance() {
    static EventDispatcher instance;
    return &instance;
}

void EventDispatcher::RegisterCollisionCallback(
    std::weak_ptr<GameObject> owner,
    CollisionCallback callback,
    CollisionFilter filter
) {
    m_CollisionCallbacks.push_back({ owner, callback, filter });
}

void EventDispatcher::DispatchCollisionEvent(const CollisionEvent& event) {
    // Remove expired entries before dispatching events
    CleanupInvalidCallbacks();

    for (auto& entry : m_CollisionCallbacks) {
        if (auto owner = entry.owner.lock()) {
            if (entry.filter(event)) {
                // Promote weak_ptr to shared_ptr to access objects securely
                if (auto objectA = event.objectA.lock()) {
                    if (auto objectB = event.objectB.lock()) {
                        GameObject* otherObject = (objectA.get() == owner.get()) ? objectB.get() : objectA.get();
                        entry.callback(otherObject, event.collisionDirection);
                    }
                }
            }
        }
    }
}

void EventDispatcher::UnregisterCallback(const GameObject* owner) {
    // Remove all entries that match the given owner
    m_CollisionCallbacks.erase(
        std::remove_if(
            m_CollisionCallbacks.begin(),
            m_CollisionCallbacks.end(),
            [owner](const CallbackEntry& entry) {
                // Check if the owner matches (ignore expired entries)
                if (auto sharedOwner = entry.owner.lock()) {
                    return sharedOwner.get() == owner;
                }
                return false; // Keep expired entries for cleanup
            }
        ),
        m_CollisionCallbacks.end()
    );
}

void EventDispatcher::CleanupInvalidCallbacks() {
    // Remove all expired callbacks
    m_CollisionCallbacks.erase(
        std::remove_if(
            m_CollisionCallbacks.begin(),
            m_CollisionCallbacks.end(),
            [](const CallbackEntry& entry) {
                return entry.owner.expired(); // Remove if the owner is no longer valid
            }
        ),
        m_CollisionCallbacks.end()
    );
}
