#include "EventDispatcher.h"
#include "../Physics/RigidBody.h"

EventDispatcher* EventDispatcher::GetInstance() {
    static EventDispatcher instance;
    return &instance;
}

void EventDispatcher::RegisterCollisionCallback(std::weak_ptr<GameObject> owner, CollisionCallback callback, CollisionFilter filter) {
    m_CollisionCallbacks.push_back({ owner, callback, filter });
}

void EventDispatcher::DispatchCollisionEvent(const CollisionEvent& event) {
    for (auto& entry : m_CollisionCallbacks) {
        if(!entry.owner.expired()){
            auto owner = entry.owner.lock(); // Try promoting weak_ptr to shared_ptr
            if (entry.filter(event)) {
                RigidBody* otherBody = (event.bodyA->GetOwner() == owner) ? event.bodyB : event.bodyA;
                entry.callback(otherBody);
            }
        }
        else {
            std::cerr << "Warning: Owner object has been destroyed" << std::endl; 
        }
    }
}

void EventDispatcher::UnregisterCallback(const std::weak_ptr<GameObject>& owner) {
    // Try promoting the weak_ptr to a shared_ptr to confirm which object is being removed
    auto ownerShared = owner.lock();
    if (!ownerShared) {
        std::cerr << "Unregister failed: owner already expired!" << std::endl;
        return; // If the object has already been destroyed, there is nothing to do
    }

    // Removes all callbacks whose owner matches the given object
    m_CollisionCallbacks.erase(
        std::remove_if(
            m_CollisionCallbacks.begin(),
            m_CollisionCallbacks.end(),
            [&ownerShared](const CallbackEntry& entry) {
                return entry.owner.lock() == ownerShared;
            }
        ),
        m_CollisionCallbacks.end()
    );
}

