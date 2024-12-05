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

CollisionLocation CollisionHandler::DetectTileCollision( Collider& entityBounds) {
    // Calculates the indexes of horizontally wrapped tiles
    int leftTile = ClampToRange( (entityBounds.GetCenterPositionX() - (entityBounds.GetWeight()/2) ) / m_MapTileSize, 0, m_MapWidth - 1);
    int rightTile = ClampToRange( (entityBounds.GetCenterPositionX() + (entityBounds.GetWeight()/2) ) / m_MapTileSize, 0, m_MapWidth - 1);

    // Calculates the indexes of vertically wrapped tiles, with circular adjustment
    int topTile = WrapTileIndex( (entityBounds.GetCenterPositionY() - (entityBounds.GetHeigth() / 2) ) / m_MapTileSize, m_MapHeight);
    int bottomTile = WrapTileIndex( (entityBounds.GetCenterPositionY() + (entityBounds.GetHeigth()/2) ) / m_MapTileSize, m_MapHeight);

    // Adjusts to avoid vertical disconnections due to circular mapping
    if (topTile > bottomTile) {
        bottomTile = topTile + ( (m_MapHeight - 1) - topTile);
    }

    // Iterates through the tiles in the calculated intervals
    for (int i = leftTile; i <= rightTile; ++i) {
        for (int j = topTile; j <= bottomTile; ++j) {
            if (m_CollisionTileMatrix[j][i] > 0) {
                return AreCloseTo(j, bottomTile) ? Below : Top;
            }
        }
    }

    return None; // No collisions detected 
}

bool CollisionHandler::AreCloseTo(int checker, int other) {
    return (checker == other || checker == other - 1);
}

int CollisionHandler::ClampToRange(int value, int min, int max) {
    return std::max(min, std::min(value, max));
}

int CollisionHandler::WrapTileIndex(int value, int mapHeight) {
    return (value % mapHeight + mapHeight) % mapHeight;
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

