#pragma once

#include "SDL.h"
#include "../Camera/Camera.h"
#include <cmath>
#include <algorithm>
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

    void SetTypeOfCollision(ObjectResponses typeOfCollision) {
        m_TypeOfCollision = typeOfCollision;
    }

    ObjectResponses GetTypeOfCollision() const {
        return m_TypeOfCollision;
    }

    std::shared_ptr<GameObject> GetOwner() const {
        return m_Owner.lock(); // Promotes weak_ptr to shared_ptr if valid
    }

    enum ColliderShape { RECTANGLE, CIRCLE };
    virtual ColliderShape GetShape() const = 0;

    void Update();

private:
    std::weak_ptr<GameObject> m_Owner;
    ObjectResponses m_TypeOfCollision;
};
