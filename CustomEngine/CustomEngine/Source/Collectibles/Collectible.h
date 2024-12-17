#pragma once

#include "../Object/GameObject.h"
#include "../Events/EventDispatcher.h"
#include "../Collision/CollisionTypes.h"
#include "../Physics/Collider.h"
#include "../Animation/SpriteAnimation.h"

struct CollectibleConfig {
    float m_CollectibleWidth;
    float m_CollectibleHeight;
};

class Collectible : public GameObject {
public:
    Collectible(const Properties& props, Transform transform);

    void InitGameObject() override;
    void Draw() override;
    void Update(float dt) override;
    void Clean() override;

    GameObjectType GetType() const override;
    Transform GetCenterPosition();
    void OnCollision(std::shared_ptr<GameObject> target);

private:
    void RegisterCollisionCallback();
    void HandleCollection();
    bool IsSameObject(const std::weak_ptr<GameObject>& a, const std::weak_ptr<GameObject>& b);

    inline Collider* GetCollider() const {
        return m_Collider.get();
    }
    void SetCollider(std::shared_ptr<Collider> collider);

    void SetColliderAsCircle(float x, float y, float radius);

    void SetColliderAsRect(float x, float y, float width, float height);

private:
    CollectibleConfig collectibleConfig;

    std::shared_ptr<Collider> m_Collider;
    SpriteAnimation* m_Animation;
};
