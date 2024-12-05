#pragma once
#include "../Physics/RigidBody.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <unordered_set>

class PhysicsWorld {
public:
    static PhysicsWorld* GetInstance() {
        static PhysicsWorld instance;
        return &instance;
    }

    // Registra um RigidBody no mundo da física
    void RegisterRigidBody(RigidBody* rigidBody) {
        m_RigidBodies.insert(rigidBody);
    }

    // Remove um RigidBody pelo ponteiro
    void UnregisterRigidBody(RigidBody* rigidBody) {
        m_RigidBodies.erase(rigidBody);
    }

    // Acesso a todos os RigidBodies ativos
    const std::unordered_set<RigidBody*>& GetRigidBodies() const {
        return m_RigidBodies;
    }

private:
    // Vetor de referências fracas para RigidBodies
    std::unordered_set<RigidBody*> m_RigidBodies;
};
