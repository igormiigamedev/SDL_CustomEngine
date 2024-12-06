#include "CollisionHandler.h"
#include "../Core/Engine.h"
#include <iostream>

CollisionHandler* CollisionHandler::s_Instance = nullptr;

CollisionHandler::CollisionHandler() {
	/*m_CollisionLayer = (TileLayer*)Engine::GetInstance()->GetMap()->GetMapLayers().back();
	m_CollisionLayer = (TileLayer*)Play::GetMap()->GetMapLayers().back();
	m_CollisionTileMap = m_CollisionLayer->GetTileMap();*/
}

void CollisionHandler::SetCollisionMap(Tile::Matrix tilematrix, int tilesize) {
	m_CollisionTileMatrix = tilematrix;
	m_MapTileSize = tilesize;
	m_MapHeight = tilematrix.size();
	m_MapWidth = tilematrix[0].size();
}

CollisionLocation CollisionHandler::DetectTileCollision( Collider& checker) {
    EdgeIndices checkerIndices = CollisionHandler::WorldPositionToMapIndex(checker);

    // Iterates through the tiles in the calculated intervals
    for (int rowIndex = checkerIndices.leftIndex; rowIndex <= checkerIndices.rightIndex; ++rowIndex) {
        for (int columnIndex = checkerIndices.topIndex; columnIndex <= checkerIndices.bottomIndex; ++columnIndex) {
            if (m_CollisionTileMatrix[columnIndex][rowIndex] > 0) {
                return AreCloseTo(columnIndex, checkerIndices.bottomIndex) ? Below : Top;
            }
        }
    }

    return None; // No collisions detected 
}

EdgeIndices CollisionHandler::WorldPositionToMapIndex(Collider& worldComponent) {
    EdgeIndices indices;

    // Collider dimensions
    const float halfWidth = worldComponent.GetWeight() / 2.0f;
    const float halfHeight = worldComponent.GetHeigth() / 2.0f;

    // Collider center
    const float centerX = worldComponent.GetCenterPositionX();
    const float centerY = worldComponent.GetCenterPositionY();

    // Calculate collider edge positions
    const float leftEdgeX = centerX - halfWidth;
    const float rightEdgeX = centerX + halfWidth;
    const float topEdgeY = centerY - halfHeight;
    const float bottomEdgeY = centerY + halfHeight;

    // Convert edge points to tile indices
    indices.leftIndex = ClampToRange(static_cast<int>(leftEdgeX / m_MapTileSize), 0, m_MapWidth - 1);
    indices.rightIndex = ClampToRange(static_cast<int>(rightEdgeX / m_MapTileSize), 0, m_MapWidth - 1);
    indices.topIndex = WrapTileIndex(static_cast<int>(topEdgeY / m_MapTileSize), m_MapHeight);
    indices.bottomIndex = WrapTileIndex(static_cast<int>(bottomEdgeY / m_MapTileSize), m_MapHeight);

    // Adjust indices to avoid vertical disconnections in circular maps
    if (indices.topIndex > indices.bottomIndex) {
        indices.bottomIndex = m_MapHeight - 1;
    }

    return indices;
}

// Function to limit the value to a specific range
int CollisionHandler::ClampToRange(int value, int min, int max) {
    return std::max(min, std::min(value, max));
}

// Function to adjust the index on circular maps
int CollisionHandler::WrapTileIndex(int value, int mapHeight) {
    return (value % mapHeight + mapHeight) % mapHeight;
}

bool CollisionHandler::AreCloseTo(int checker, int other) {
    return (checker == other || checker == other - 1);
}


bool CollisionHandler::CheckCollision(Collider& checker, Collider& other) {
    if (checker.GetType() == Collider::ColliderType::RECTANGLE && other.GetType() == Collider::ColliderType::RECTANGLE) {
        auto* rectA = dynamic_cast<RectCollider*>(&checker);
        auto* rectB = dynamic_cast<RectCollider*>(&other);
        if (rectA && rectB) {
            return SDL_HasIntersection(&rectA->GetBox(), &rectB->GetBox());
        }
    }
    else if (checker.GetType() == Collider::ColliderType::CIRCLE && other.GetType() == Collider::ColliderType::CIRCLE) {
        auto* circleA = dynamic_cast<CircleCollider*>(&checker);
        auto* circleB = dynamic_cast<CircleCollider*>(&other);
        if (circleA && circleB) {
            return CheckCircleCollision(*circleA, *circleB);
        }
    }
    else if (checker.GetType() == Collider::ColliderType::RECTANGLE && other.GetType() == Collider::ColliderType::CIRCLE) {
        auto* rect = dynamic_cast<RectCollider*>(&checker);
        auto* circle = dynamic_cast<CircleCollider*>(&other);
        if (rect && circle) {
            return CheckRectCircleCollision(*circle, *rect);
        }
    }
    else if (checker.GetType() == Collider::ColliderType::CIRCLE && other.GetType() == Collider::ColliderType::RECTANGLE) {
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

