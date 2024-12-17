#pragma once

#include "SDL.h"
#include "../Camera/Camera.h"
#include <cmath>
#include <algorithm>
#include "../Core/Engine.h"
#include "../Collision/CollisionManager.h"

class Collider : public CollisionManager {
public:
    
    Collider();

    virtual ~Collider();
    /*virtual ~Collider() = default;*/

    virtual void DrawDebugCollider() = 0;

    virtual void SetPositionX(float x) = 0;
    virtual void SetPositionY(float y) = 0;

    virtual float GetCenterPositionX() = 0;
    virtual float GetCenterPositionY() = 0;

    virtual float GetWeight() = 0;
    virtual float GetHeigth() = 0;

    void SetOwner(const std::shared_ptr<GameObject>& owner) {
        m_Owner = owner; // Sets the owner to weak_ptr
    }

    std::shared_ptr<GameObject> GetOwner() const {
        return m_Owner.lock(); // Promotes weak_ptr to shared_ptr if valid
    }

    enum ColliderType { RECTANGLE, CIRCLE };
    virtual ColliderType GetType() const = 0;

    void Update();

private:
    std::weak_ptr<GameObject> m_Owner;
};
