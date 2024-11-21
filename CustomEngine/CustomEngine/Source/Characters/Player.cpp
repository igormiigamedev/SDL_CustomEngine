#include "Player.h"
#include "../Graphics/TextureManager.h"
#include "../Inputs/InputHandler.h"
#include "SDL.h"
#include "../Core/Engine.h" // TODO - Remove when update Player Flip logic
#include "../Camera/Camera.h"
#include "../Collision/CollisionHandler.h"
#include "../Factory/ObjectFactory.h"

static RegisterObject<Player> registerObject(GameObjectType::PLAYER);

Player::Player(const Properties& props, Transform transform) : Character(props, transform){

	m_RigidBody = new RigidBody();
	m_RigidBody->SetGravity(360.0f);
	m_RigidBody->GetCollider()->SetBuffer(0, 0, 0, 0);

	m_Animation = new SpriteAnimation();
	m_Animation->SetProps(m_TextureID, playerConfig.m_PlayerWalkSpriteRow, playerConfig.m_PlayerWalkFrameCount, playerConfig.m_PlayerWalkAnimSpeed);

	playerConfig.m_PlayerWidth = m_Properties.ScaleX * (m_Properties.Width / playerConfig.m_PlayerWalkFrameCount);
	playerConfig.m_PlayerHeight = m_Properties.ScaleY * (m_Properties.Height / playerConfig.m_PlayerWalkSpriteRow);
}

void Player::Draw(){
	m_Animation->Draw(m_Transform.X, m_Transform.Y, m_Properties.Width, m_Properties.Height, m_Properties.ScaleX, m_Properties.ScaleY, m_Flip);
	m_RigidBody->GetCollider()->SetProperties(m_Transform.X, m_Transform.Y, playerConfig.m_PlayerWidth, (playerConfig.m_PlayerHeight - 30));

	m_RigidBody->GetCollider()->DrawDebugCollider();
}

void Player::Clean(){
	TextureManager::GetInstance()->Drop(m_TextureID);
}

void Player::Update(float dt){
	//std::cout << "Valor de dt: " << dt << std::endl;

	WalkMovement(dt);
	JumpMovement(dt);
	/*m_RigidBody->GetCollider()->SetProperties(m_Transform->X, m_Transform->Y, playerConfig.m_PlayerWidth, (playerConfig.m_PlayerHeight - 30));*/
	AnimationState();

	m_Origin->X = m_Transform.X;// + playerConfig.m_PlayerWidth  / 2;
	m_Origin->Y = m_Transform.Y;// + playerConfig.m_PlayerHeight / 2;

	m_Animation->Update(dt);
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
	if (m_Transform.X >= (SCREEN_WIDTH - playerConfig.m_PlayerWidth)) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
		playerDirection = BACKWARD;
		m_Flip = SDL_FLIP_HORIZONTAL;
	}
	else if (m_Transform.X <= 0) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
		playerDirection = FORWARD;
		m_Flip = SDL_FLIP_NONE;
	}
}

void Player::ApplyWalkingForce(float dt) {
	m_RigidBody->SetVelocityX(WALK_VELOCITY * playerDirection);
	m_RigidBody->Update(dt);
}

void Player::UpdatePlayerPositionX(float dt) {
	m_Transform.X += m_RigidBody->GetDeltaPosition().X;
	m_RigidBody->GetCollider()->SetPositionX(m_Transform.X);
}

void Player::JumpMovement(float dt) {
	HandleJumpInput();
	HandleFallingState();
	UpdatePlayerPositionY(dt);
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
	if (m_IsJumping) {
		m_RigidBody->SetVelocityY(UPWARD * JUMP_VELOCITY);
		m_RigidBody->GetCollider()->SetCollisionResponse(WorldFloor, OVERLAP);
	}
}

void Player::HandleFallingState() {
	if (m_RigidBody->GetVelocity().Y > 0 && !m_IsGrounded) {
		m_RigidBody->GetCollider()->SetCollisionResponse(WorldFloor, BLOCK);
		m_IsFalling = true;
		m_IsJumping = false;
	}
	else{
		if (m_RigidBody->GetVelocity().Y == 0) {
			m_IsGrounded = true;
			m_IsJumping = false;
			m_UsedDoubleJump = false;
		}
		m_IsFalling = false;
	}
}

void Player::UpdatePlayerPositionY(float dt) {
	m_RigidBody->Update(dt);
	m_Transform.Y += m_RigidBody->GetDeltaPosition().Y;
	m_RigidBody->GetCollider()->SetPositionY(m_Transform.Y);
}

bool Player::IsSpacePressed() const {
	return InputHandler::GetInstance()->GetKeyPressed(SDL_SCANCODE_SPACE);
}

void Player::AnimationState(){

	m_Animation->SetProps("Player_Walk", playerConfig.m_PlayerWalkSpriteRow, playerConfig.m_PlayerWalkFrameCount, playerConfig.m_PlayerWalkAnimSpeed);
	/*m_SpriteSheetWidth = 240;
	m_SpriteSheetHeight = 207;*/

	if (m_IsJumping) {
		//m_ImageScalling = 2;
		/*m_SpriteSheetWidth = 1500;
		m_SpriteSheetHeight = 500;
		m_Animation->SetProps("Pombo_Jump", 1, 3, 180);*/
		m_Animation->SetProps("Player_Jump", 1, 2, 180);
	}

	if (m_IsFalling) {
		/*m_SpriteSheetWidth = 1500;
		m_SpriteSheetHeight = 500;
		m_Animation->SetProps("Pombo_Jump", 1, 3, 180);*/
		m_Animation->SetProps("Player_Jump", 1, 2, 180);
	}

}
