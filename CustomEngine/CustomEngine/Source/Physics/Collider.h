#pragma once

#include "SDL.h"
#include "../Camera/Camera.h"
#include <cmath>
#include <algorithm>
#include "../Core/Engine.h"
#include "../Collision/CollisionManager.h"

class Collider : public CollisionManager {
public:
    virtual ~Collider() = default;

    virtual void DrawDebugCollider() = 0;

    virtual void SetPositionX(float x) = 0;
    virtual void SetPositionY(float y) = 0;

    virtual float GetCenterPositionX() = 0;
    virtual float GetCenterPositionY() = 0;

    virtual float GetWeight() = 0;
    virtual float GetHeigth() = 0;

    enum ColliderType { RECTANGLE, CIRCLE };
    virtual ColliderType GetType() const = 0;
};
