#include "CollisionHandler.h"
#include "../Core/Engine.h"
#include <iostream>
#include "PhysicsWorld.h"
#include "../Events/EventDispatcher.h"

CollisionHandler* CollisionHandler::s_Instance = nullptr;

CollisionHandler::CollisionHandler() {
	/*m_CollisionLayer = (TileLayer*)Engine::GetInstance()->GetMap()->GetMapLayers().back();
	m_CollisionLayer = (TileLayer*)Play::GetMap()->GetMapLayers().back();
	m_CollisionTileMap = m_CollisionLayer->GetTileMap();*/
}

bool CollisionHandler::CheckCollision(Collider& checker, Collider& other) {
    if (checker.GetShape() == Collider::ColliderShape::RECTANGLE && other.GetShape() == Collider::ColliderShape::RECTANGLE) {
        auto* rectA = dynamic_cast<RectCollider*>(&checker);
        auto* rectB = dynamic_cast<RectCollider*>(&other);
        if (rectA && rectB) {
            return SDL_HasIntersection(&rectA->GetBox(), &rectB->GetBox());
        }
    }
    else if (checker.GetShape() == Collider::ColliderShape::CIRCLE && other.GetShape() == Collider::ColliderShape::CIRCLE) {
        auto* circleA = dynamic_cast<CircleCollider*>(&checker);
        auto* circleB = dynamic_cast<CircleCollider*>(&other);
        if (circleA && circleB) {
            return CheckCircleCollision(*circleA, *circleB);
        }
    }
    else if (checker.GetShape() == Collider::ColliderShape::RECTANGLE && other.GetShape() == Collider::ColliderShape::CIRCLE) {
        auto* rect = dynamic_cast<RectCollider*>(&checker);
        auto* circle = dynamic_cast<CircleCollider*>(&other);
        if (rect && circle) {
            return CheckRectCircleCollision(*circle, *rect);
        }
    }
    else if (checker.GetShape() == Collider::ColliderShape::CIRCLE && other.GetShape() == Collider::ColliderShape::RECTANGLE) {
        auto* circle = dynamic_cast<CircleCollider*>(&checker);
        auto* rect = dynamic_cast<RectCollider*>(&other);
        if (circle && rect) {
            return CheckRectCircleCollision(*circle, *rect);
        }
    }
    return false;
}

bool CollisionHandler::CheckRectCircleCollision(const CircleCollider& circleCollider, const RectCollider& rectCollider) const {
    int closestX = std::max(rectCollider.GetBox().x,
        std::min(static_cast<int>(circleCollider.GetCircle().x), rectCollider.GetBox().x + rectCollider.GetBox().w));
    int closestY = std::max(rectCollider.GetBox().y,
        std::min(static_cast<int>(circleCollider.GetCircle().y), rectCollider.GetBox().y + rectCollider.GetBox().h));

    int dx = static_cast<int>(circleCollider.GetCircle().x) - closestX;
    int dy = static_cast<int>(circleCollider.GetCircle().y) - closestY;
    return (dx * dx + dy * dy) <= (circleCollider.GetCircle().radius * circleCollider.GetCircle().radius);
}

bool CollisionHandler::CheckCircleCollision(const CircleCollider& checker, const CircleCollider& other) const {
    float dx = checker.GetCircle().x - other.GetCircle().x;
    float dy = checker.GetCircle().y - other.GetCircle().y;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = checker.GetCircle().radius + other.GetCircle().radius;
    return distanceSquared <= radiusSum * radiusSum;
}

void CollisionHandler::CheckCollisionsWithCollidersOfType(Collider& sourceCollider, ObjectResponses typeOfCollision) {
    // Get all valid colliders from PhysicsWorld
    auto colliders = PhysicsWorld::GetInstance()->GetColliders();

    // Iterate over colliders and check for collisions
    for (const auto& targetCollider : colliders) {
        // Ignore collision with Collider itself
        if (targetCollider.get() == &sourceCollider) continue;

        if (targetCollider->GetTypeOfCollision() != typeOfCollision) continue;

        // Check collision between sourceCollider and targetCollider
        if (CheckCollision(sourceCollider, *targetCollider)) {
            // Get collider owners
            auto ownerA = sourceCollider.GetOwner();  
            auto ownerB = targetCollider.get()->GetOwner(); 

            // Check if owners are still valid
            if (ownerA && ownerB) {
                float sourceY = sourceCollider.GetCenterPositionY() + sourceCollider.GetHeigth()/2;
                float targetY = targetCollider->GetCenterPositionY();

                // Sets the collision direction
                CollisionDirection direction = (sourceY <= targetY) ? CollisionDirection::BELOW : CollisionDirection::ABOVE;

                // Create the event with the correct direction
                CollisionEvent event(ownerA, ownerB, direction);
                EventDispatcher::GetInstance()->DispatchCollisionEvent(event);
            }
        }
    }
}

