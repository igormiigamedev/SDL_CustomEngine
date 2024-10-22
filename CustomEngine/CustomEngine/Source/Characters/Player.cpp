#include "Player.h"
#include "../Graphics/TextureManager.h"
#include "../Inputs/InputHandler.h"
#include "SDL.h"
#include "../Core/Engine.h" // TODO - Remove when update Player Flip logic
#include "../Camera/Camera.h"
#include "../Collision/CollisionHandler.h"

Player::Player(Properties* props) : Character(props){

	m_JumpTime = JUMP_TIME;
	m_JumpForce = JUMP_FORCE;

	m_Collider = new Collider();
	m_Collider->SetBuffer(0, 0, 0, 0);

	m_RigidBody = new RigidBody();
	m_RigidBody->SetGravity(9.0f);

	m_Animation = new Animation();
	m_Animation->SetProps(m_TextureID, playerConfig.m_PlayerWalkSpriteRow, playerConfig.m_PlayerWalkFrameCount, playerConfig.m_PlayerWalkAnimSpeed);
}

void Player::Draw(){
	m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);

	m_Collider->DrawDebugCollider();
}

void Player::Clean(){
	TextureManager::GetInstance()->Drop(m_TextureID);
}

void Player::Update(float dt){
	//std::cout << "Valor de dt: " << dt << std::endl;
	/*m_Animation->SetProps("Player_Walk", 1, 2, 100);*/ 

	WalkMovement(dt);
	JumpMovement(dt);

	m_Origin->X = m_Transform->X + m_Width / 2;
	m_Origin->Y = m_Transform->Y + m_Height / 2;

	m_Animation->Update();
}

void Player::WalkMovement(float dt) {

	if (m_Transform->X >= (SCREEN_WIDTH - 120)) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
		playerDirection = BACKWARD;
		m_Animation->SetProps("Player_Walk", playerConfig.m_PlayerWalkSpriteRow, playerConfig.m_PlayerWalkFrameCount, playerConfig.m_PlayerWalkAnimSpeed, SDL_FLIP_HORIZONTAL);
	}
	else if (m_Transform->X <= 0) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
		playerDirection = FORWARD;
		m_Animation->SetProps("Player_Walk", playerConfig.m_PlayerWalkSpriteRow, playerConfig.m_PlayerWalkFrameCount, playerConfig.m_PlayerWalkAnimSpeed);
	}

	m_RigidBody->ApplyForceX(12 * playerDirection);
	m_RigidBody->Update(dt);
	m_LastSafePosition.X = m_Transform->X;
	m_Transform->X += m_RigidBody->GetDeltaPosition().X;
	m_Collider->Set(m_Transform->X, m_Transform->Y, 160, 210);

	/*if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_Transform->X = m_LastSafePosition.X;
	}*/
}

void Player::JumpMovement(float dt) {
	std::cout << "m_IsGrounded: " << m_IsGrounded << std::endl;

	// TODO - Double Jump
	/*if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE) && !m_IsGrounded && !m_UsedDoubleJump) {
 		m_UsedDoubleJump = true;
		m_IsJumping = true;
		m_IsGrounded = false;
		m_JumpTime = JUMP_TIME;
		m_RigidBody->ApplyForceY(UPWARD * (m_JumpForce * 2));
	}*/
	if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE) && m_IsGrounded) {
		m_IsJumping = true;
		m_IsGrounded = false;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce); 
	}
	if (m_IsJumping && m_JumpTime > 0) {
		m_JumpTime -= dt;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
	}
	else {
		m_IsJumping = false;
		m_JumpTime = JUMP_TIME;
		m_RigidBody->UnSetForceY();
	}
	
	m_RigidBody->Update(dt);
	m_LastSafePosition.Y = m_Transform->Y;
	m_Transform->Y += m_RigidBody->GetDeltaPosition().Y;
	m_Collider->Set(m_Transform->X, m_Transform->Y, 160, 210);

	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_IsGrounded = true;
		m_Transform->Y = m_LastSafePosition.Y;
		/*m_UsedDoubleJump = false;*/
	}
	else {
		m_IsGrounded = false;
	}

	// TODO - Delete and review when doing the anim state
	if (m_IsJumping || !m_IsGrounded) {
		m_Animation->SetProps("Player_Jump", 1, 2, 180);
	}
	else {
		if (SDL_FLIP_HORIZONTAL == playerDirection) {
			m_Animation->SetProps("Player_Walk", playerConfig.m_PlayerWalkSpriteRow, playerConfig.m_PlayerWalkFrameCount, playerConfig.m_PlayerWalkAnimSpeed);
		}
		else {
			m_Animation->SetProps("Player_Walk", playerConfig.m_PlayerWalkSpriteRow, playerConfig.m_PlayerWalkFrameCount, playerConfig.m_PlayerWalkAnimSpeed, SDL_FLIP_HORIZONTAL);
		}
	}
}
