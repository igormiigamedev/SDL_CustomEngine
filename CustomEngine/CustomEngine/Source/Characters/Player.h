#pragma once

#include "Character.h"
#include "../Animation/SpriteAnimation.h"
#include "../Physics/Vector2D.h"
#include "../GameMode/GameMode.h"

#define JUMP_VELOCITY 750.0f  //680.0f 

#define WALK_VELOCITY 520.0f 

struct PlayerConfig {
	float m_PlayerWidth;
	float m_PlayerHeight;
};

class Player : public Character{

	public:
		Player(const Properties& props, Transform transform);

		GameObjectType GetType() const override { return GameObjectType::PLAYER; };

		virtual void Draw();
		virtual void Clean();
		virtual void Update(float dt);
		Transform GetBodyCenterPosition();
		virtual void OnTakeDamage(float damage);
		virtual void OnCollision(std::shared_ptr<GameObject> target, CollisionDirection direction);

		//Walk
		void WalkMovement(float dt);

		// Jump
		void JumpMovement(float dt);

		bool IsDead() { return m_IsDead; };
		

	private:
		//Walk
		void UpdatePlayerDirection();
		void ApplyWalkingForce(float dt);
		void UpdatePlayerPositionX(float dt);

		// Jump
		void CheckDoubleJump();
		void AnimationState();
		void HandleJumpInput();
		void StartDoubleJump();
		void StartJump();
		void ApplyJumpForce(float dt = 0);
		void HandleFallingState();
		void UpdatePlayerPositionY(float dt);
		bool IsSpacePressed() const;

		//Death
		void HandlePlayerDeath();

	private:
		PlayerConfig playerConfig;

		bool m_IsDead = false;
		bool m_IsJumping;
		bool m_IsGrounded;
		bool m_IsFalling;
		bool m_UsedDoubleJump = false;

		SpriteAnimation* m_Animation;

		int playerDirection = 1;

};