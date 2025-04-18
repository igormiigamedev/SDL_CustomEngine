#pragma once

#include "Character.h"
#include "../Animation/SequenceAnimation.h"
#include "../Animation/SpriteAnimation.h"

#define WALK_VELOCITY 280.0f

struct EnemyConfig {
	float m_EnemyWidth;
	float m_EnemyHeight;
	float tailWidth;
	float bodyWidth;
};

class Enemy : public Character{

public:
	Enemy(const Properties& props, Transform transform);

	GameObjectType GetType() const override { return GameObjectType::ENEMY; };
	
	virtual void Draw();
	virtual void Clean();
	virtual void Update(float dt);
	Transform GetBodyCenterPosition();
	virtual void OnTakeDamage(float damage);
	virtual void OnCollision(std::shared_ptr<GameObject> target, CollisionDirection direction);

private:
	//Walk
	void WalkMovement(float dt);
	void UpdateCharacterDirection();
	void ApplyWalkingForce(float dt);
	void UpdateCharacterPositionX(float dt);

	void UpdateCharacterPositionY(float dt);

private:
	EnemyConfig enemyConfig;

	SequenceAnimation* m_Animation;

	int characterDirection = 1;
};

