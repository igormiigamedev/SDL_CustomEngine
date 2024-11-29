#pragma once
#include "../Physics/RigidBody.h"  
#include <vector>
#include <memory>

class PhysicsWorld {
public:
    static PhysicsWorld* GetInstance() {
        static PhysicsWorld instance;
        return &instance;
    }

    void RegisterRigidBody(std::shared_ptr<RigidBody> body) {
        m_RigidBodies.push_back(body);
    }

    void UnregisterRigidBody(const std::shared_ptr<RigidBody>& body) {
        m_RigidBodies.erase(std::remove(m_RigidBodies.begin(), m_RigidBodies.end(), body), m_RigidBodies.end());
    }

    const std::vector<std::shared_ptr<RigidBody>>& GetRigidBodies() const {
        return m_RigidBodies;
    }

private:
    std::vector<std::shared_ptr<RigidBody>> m_RigidBodies;
};



