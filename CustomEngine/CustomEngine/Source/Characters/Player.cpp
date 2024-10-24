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

	playerConfig.m_PlayerWidth = m_ImageScalling * (m_SpriteSheetWidth / playerConfig.m_PlayerWalkFrameCount);
	playerConfig.m_PlayerHeight = m_ImageScalling * (m_SpriteSheetHeight / playerConfig.m_PlayerWalkSpriteRow);
}

void Player::Draw(){
	m_Animation->Draw(m_Transform->X, m_Transform->Y, m_SpriteSheetWidth, m_SpriteSheetHeight, m_ImageScalling, m_Flip);

	/*m_Collider->DrawDebugCollider();*/
}

void Player::Clean(){
	TextureManager::GetInstance()->Drop(m_TextureID);
}

void Player::Update(float dt){
	//std::cout << "Valor de dt: " << dt << std::endl;

	WalkMovement(dt);
	JumpMovement(dt);
	AnimationState();

	m_Origin->X = m_Transform->X;// + playerConfig.m_PlayerWidth  / 2;
	m_Origin->Y = m_Transform->Y;// + playerConfig.m_PlayerHeight / 2;

	m_Animation->Update();
}

void Player::WalkMovement(float dt) {

	UpdatePlayerDirection();
	ApplyWalkingForce(dt);
	UpdatePlayerPositionX(dt);

	/*m_Collider->Set(m_Transform->X, m_Transform->Y, playerConfig.m_PlayerWidth, playerConfig.m_PlayerHeight);
	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_Transform->X = m_LastSafePosition.X;
	}*/
}

void Player::UpdatePlayerDirection() {
	if (m_Transform->X >= (SCREEN_WIDTH - 120)) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
		playerDirection = BACKWARD;
		m_Flip = SDL_FLIP_HORIZONTAL;
	}
	else if (m_Transform->X <= 0) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
		playerDirection = FORWARD;
		m_Flip = SDL_FLIP_NONE;
	}
}

void Player::ApplyWalkingForce(float dt) {
	m_RigidBody->ApplyForceX(WALK_FORCE * playerDirection);
	m_RigidBody->Update(dt);
}

void Player::UpdatePlayerPositionX(float dt) {
	m_LastSafePosition.X = m_Transform->X;
	m_Transform->X += m_RigidBody->GetDeltaPosition().X;
}

void Player::JumpMovement(float dt) {
	HandleJumpInput();
	ApplyJumpForce(dt);
	HandleFallingState();
	UpdatePlayerPositionY(dt);
	CheckCollisionAndReset();
}

void Player::HandleJumpInput() {
	if (IsSpacePressed() && !m_IsGrounded && !m_UsedDoubleJump) {
		StartDoubleJump();
	}
	if (IsSpacePressed() && m_IsGrounded) {
		StartJump();
	}
}

void Player::StartDoubleJump() {
	m_UsedDoubleJump = true;
	m_JumpTime = JUMP_TIME;
	m_IsJumping = true;
	m_IsGrounded = false;
	ApplyJumpForce();
}

void Player::StartJump() {
	m_IsJumping = true;
	m_IsGrounded = false;
	ApplyJumpForce();
}

void Player::ApplyJumpForce(float dt) {
	if (m_IsJumping && m_JumpTime > 0) {
		m_JumpTime -= dt;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
	}
	else {
		StopJump();
	}
}

void Player::StopJump() {
	m_IsJumping = false;
	m_JumpTime = JUMP_TIME;
	m_RigidBody->UnSetForceY();
}

void Player::HandleFallingState() {
	if (m_RigidBody->GetVelocity().Y > 0 && !m_IsGrounded) {
		m_IsFalling = true;
	}
	else {
		m_IsFalling = false;
	}
}

void Player::UpdatePlayerPositionY(float dt) {
	m_RigidBody->Update(dt);
	m_LastSafePosition.Y = m_Transform->Y;
	m_Transform->Y += m_RigidBody->GetDeltaPosition().Y;
	m_Collider->Set(m_Transform->X, m_Transform->Y, playerConfig.m_PlayerWidth, (playerConfig.m_PlayerHeight - 30));
}

void Player::CheckCollisionAndReset() {
	if (!m_IsJumping && CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
		m_IsGrounded = true;
		m_Transform->Y = m_LastSafePosition.Y;
		m_UsedDoubleJump = false;
	}
	else {
		m_IsGrounded = false;
	}
}

bool Player::IsSpacePressed() const {
	return InputHandler::GetInstance()->GetKeyPressed(SDL_SCANCODE_SPACE);
}

void Player::AnimationState(){

	m_Animation->SetProps("Player_Walk", playerConfig.m_PlayerWalkSpriteRow, playerConfig.m_PlayerWalkFrameCount, playerConfig.m_PlayerWalkAnimSpeed);

	if (m_IsJumping) {
		m_Animation->SetProps("Player_Jump", 1, 2, 180);
	}

	if (m_IsFalling) {
		m_Animation->SetProps("Player_Jump", 1, 2, 180);
	}

}
