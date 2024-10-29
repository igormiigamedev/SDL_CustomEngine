#pragma once

#include "Character.h"
#include "../Animation/Animation.h"
#include "../Physics/RigidBody.h"
#include "../Physics/Collider.h"
#include "../Physics/Vector2D.h"

#define JUMP_TIME 3.0f
#define JUMP_FORCE 120.0f
#define JUMP_VELOCITY 450.0f //400.0f

#define WALK_FORCE 12.0f
#define WALK_VELOCITY 400.0f

struct PlayerConfig {
	/*std::string m_PlayerTextureID;*/  
	int m_PlayerWalkSpriteRow = 1;      
	int m_PlayerWalkFrameCount = 2;     
	int m_PlayerWalkAnimSpeed = 100;   
	float m_PlayerWidth;
	float m_PlayerHeight;
	/*float m_PlayerTextureScalling = 0.7;*/
};


class Player : public Character{

	public:
		Player(Properties* props);

		virtual void Draw();
		virtual void Clean();
		virtual void Update(float dt);

		//Walk
		void WalkMovement(float dt);

		// Jump
		void JumpMovement(float dt);
		

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

	private:
		PlayerConfig playerConfig;

		bool m_IsJumping;
		bool m_IsGrounded;
		bool m_IsFalling;
		bool m_UsedDoubleJump = false;

		Animation* m_Animation;
		RigidBody* m_RigidBody;

		int playerDirection = 1;
		

};