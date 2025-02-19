#include "Collider.h"
#include "../Collision/CollisionHandler.h"
#include "../Collision/PhysicsWorld.h"

Collider::Collider() {}

Collider::~Collider() {}

void Collider::Update() {
    switch (this->GetCollisionResponse(PhysicsBody)) {
    case IGNORE:
        break;
    case BLOCK:
        break;
    case OVERLAP:
        CollisionHandler::GetInstance()->CheckCollisionsWithCollidersOfType(*this, PhysicsBody);
        break;
    default:
        break;
    }

    switch (this->GetCollisionResponse(WorldFloor)) {
    case IGNORE:
        break;
    case BLOCK:
        CollisionHandler::GetInstance()->CheckCollisionsWithCollidersOfType(*this, WorldFloor);
        break;
    case OVERLAP:
        // Handle overlap case if needed
        break;
    default:
        break;
    }
}