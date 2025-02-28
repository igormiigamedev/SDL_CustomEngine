#include "Collectible.h"
#include <iostream>
#include "../Collision/PhysicsWorld.h"
#include "../Physics/CircleCollider.h"
#include "../Physics/RectCollider.h"

static RegisterObject<Collectible> registerObject(GameObjectType::COLLECTIBLE);

Collectible::Collectible(const Properties& props, Transform transform)
    : GameObject(props, transform) {
    m_Animation = new SpriteAnimation();
    m_Animation->Parse();
    m_Flip = SDL_FLIP_HORIZONTAL;
    m_Animation->SetAnimation("Bread_Idle");

    collectibleConfig.m_CollectibleWidth = m_Properties.ScaleX * m_Animation->GetCurrentSpriteWidth();
    collectibleConfig.m_CollectibleHeight = m_Properties.ScaleY * m_Animation->GetCurrentSpriteHeight();

    float radius = (collectibleConfig.m_CollectibleHeight - 80) / 2;
    const auto& center = GetCenterPosition();
    SetColliderAsCircle(center.X, center.Y, radius);
}

void Collectible::InitGameObject() {

    m_Collider->SetOwner(shared_from_this());

    RegisterCollisionCallback();
}

void Collectible::Draw() {
    m_Animation->Draw(m_Transform.X, m_Transform.Y, m_Properties.ScaleX, m_Properties.ScaleY, m_Flip);
    /*m_Collider->DrawDebugCollider();*/
   /* std::cout << "Drawing collectible at (" << m_Transform.X << ", " << m_Transform.Y << ")\n";*/
}

void Collectible::Update(float dt) {
    m_Animation->Update(dt);
}

void Collectible::Clean() {
    std::cout << "Collectible cleaned up\n";
}

GameObjectType Collectible::GetType() const {
    return GameObjectType::COLLECTIBLE; 
}

Transform Collectible::GetCenterPosition() {
    Transform centerTransform;

    // Adjust to ignore the non-body part of the image
    constexpr float imageTopMargin = 35.0f;

    // Calculates the X and Y position of the center of the body
    centerTransform.X = m_Transform.X + (collectibleConfig.m_CollectibleWidth / 2.0f);
    centerTransform.Y = m_Transform.Y + (collectibleConfig.m_CollectibleHeight / 2.0f) + imageTopMargin;

    return centerTransform;
}

void Collectible::OnCollision(std::shared_ptr<GameObject> target) {
    if (target->GetType() == GameObjectType::PLAYER) {
        std::cout << "Collectible collected by Player!\n";

        // Performs specific logic (scoring, removing, etc.)
        HandleCollection();

        // Mark for removal
        m_IsActive = false;
    }
}

void Collectible::RegisterCollisionCallback() {
    EventDispatcher::GetInstance()->RegisterCollisionCallback(
        shared_from_this(),
        [this](GameObject* otherObject, CollisionDirection direction) {
            if (auto target = otherObject->GetWeakPtr().lock()) {
                OnCollision(target);
            }
        },
        [this](const CollisionEvent& event) {
            auto self = shared_from_this();
            return IsSameObject(event.objectA, self) || IsSameObject(event.objectB, self);
        }
    );
}

void Collectible::HandleCollection() {
    std::cout << "Handle collection logic here.\n";

    std::shared_ptr<ParticleEffects> particle = std::make_shared<ParticleEffects>();
    particle->SpawnParticle(m_Transform, "Bread_Collected", m_Properties.ScaleX, m_Properties.ScaleY, false);
}

bool Collectible::IsSameObject(const std::weak_ptr<GameObject>& a, const std::weak_ptr<GameObject>& b) {
    return !a.owner_before(b) && !b.owner_before(a);
}

void Collectible::SetCollider(std::shared_ptr<Collider> collider) {
    m_Collider = std::move(collider);
    PhysicsWorld::GetInstance()->RegisterCollider(m_Collider);
}

void Collectible::SetColliderAsCircle(float x, float y, float radius) {
    SetCollider(std::make_unique<CircleCollider>(x, y, radius, WorldDynamic));
}

void Collectible::SetColliderAsRect(float x, float y, float width, float height) {
    SetCollider(std::make_unique<RectCollider>(x, y, width, height, WorldDynamic));
}