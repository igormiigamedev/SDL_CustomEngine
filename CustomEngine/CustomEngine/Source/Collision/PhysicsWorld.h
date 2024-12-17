#pragma once
#include <functional> // For std::hash
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_set>

class RigidBody;

class PhysicsWorld {
public:
    static PhysicsWorld* GetInstance() {
        static PhysicsWorld instance;
        return &instance;
    }

    void Update() {
        CleanUpInvalidColliders();

        for (auto collider : GetColliders()) {
            collider.get()->Update();
        }
    }

    void RegisterCollider(std::shared_ptr<Collider> collider) {
        if (collider) {
            m_Colliders.insert(collider);
        }
    }

    void CleanUpInvalidColliders() {
        for (auto it = m_Colliders.begin(); it != m_Colliders.end(); ) {
            if (it->expired()) {
                it = m_Colliders.erase(it); 
            }
            else {
                ++it;
            }
        }
    }

    // Get all valid Colliders
    std::vector<std::shared_ptr<Collider>> GetColliders() {
        CleanUpInvalidColliders(); 

        std::vector<std::shared_ptr<Collider>> validColliders;

        for (const auto& weakCollider : m_Colliders) {
            if (auto collider = weakCollider.lock()) {
                validColliders.push_back(collider);
            }
        }

        return validColliders;
    }


    struct WeakPtrHash {
        template <typename T>
        std::size_t operator()(const std::weak_ptr<T>& weakPtr) const {
            auto shared = weakPtr.lock();
            return shared ? std::hash<std::shared_ptr<T>>{}(shared) : 0;
        }
    };

    struct WeakPtrEqual {
        template <typename T>
        bool operator()(const std::weak_ptr<T>& a, const std::weak_ptr<T>& b) const {
            return !a.owner_before(b) && !b.owner_before(a);
        }
    };


private:
    // Weak reference set for Colliders
    std::unordered_set<std::weak_ptr<Collider>, WeakPtrHash, WeakPtrEqual> m_Colliders;
};
