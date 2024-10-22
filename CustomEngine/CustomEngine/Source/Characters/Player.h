#pragma once

#include "Character.h"
#include "../Animation/Animation.h"
#include "../Physics/RigidBody.h"
#include "../Physics/Collider.h"
#include "../Physics/Vector2D.h"

#define JUMP_TIME 2.8f
#define JUMP_FORCE 180.0f

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

		void WalkMovement(float dt);

		void JumpMovement(float dt);

	private:
		PlayerConfig playerConfig;

		bool m_IsJumping;
		bool m_IsGrounded;

		float m_JumpTime;
		float m_JumpForce;

		Collider* m_Collider;

		Animation* m_Animation;
		RigidBody* m_RigidBody;
		Vector2D m_LastSafePosition;

		int playerDirection = 1;
		/*bool m_UsedDoubleJump = false;*/

};