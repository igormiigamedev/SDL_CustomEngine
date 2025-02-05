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

        for (auto collider : m_Colliders) {
            if (auto validCollider = collider.lock()) {
                validCollider->Update();
            }
        }
    }

    void Reset() {
        m_Colliders.clear();
    }


    void RegisterCollider(std::shared_ptr<Collider> collider) {
        if (!collider) return;

        m_Colliders.push_back(collider);
    }

    void CleanUpInvalidColliders() {
        // Remove colliders expirados do vetor
        m_Colliders.erase(
            std::remove_if(m_Colliders.begin(), m_Colliders.end(),
                [](const std::weak_ptr<Collider>& weakCollider) {
                    return weakCollider.expired();
                }),
            m_Colliders.end());
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

private:
    std::vector<std::weak_ptr<Collider>> m_Colliders;
};
