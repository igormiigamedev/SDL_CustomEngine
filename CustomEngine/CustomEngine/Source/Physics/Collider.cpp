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
        CollisionHandler::GetInstance()->CheckCollisionsWithColliders(*this);
        break;
    default:
        break;
    }
}