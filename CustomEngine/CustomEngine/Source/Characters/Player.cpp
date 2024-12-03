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
	RegisterCollisionCallback();

	GetRigidBody()->SetGravity(700.0f);

	m_Animation = new SpriteAnimation();
	m_Animation->SetProps(m_TextureID, playerConfig.m_PlayerWalkSpriteRow, playerConfig.m_PlayerWalkFrameCount, playerConfig.m_PlayerWalkAnimSpeed);

	playerConfig.m_PlayerWidth = m_Properties.ScaleX * (m_Properties.Width / playerConfig.m_PlayerWalkFrameCount);
	playerConfig.m_PlayerHeight = m_Properties.ScaleY * (m_Properties.Height / playerConfig.m_PlayerWalkSpriteRow);

	float radius = (playerConfig.m_PlayerHeight - 80) / 2;
	auto center = GetBodyCenterPosition();
	SetColliderAsCircle(center.X, center.Y, radius);
}

void Player::Draw(){
	m_Animation->Draw(m_Transform.X, m_Transform.Y, m_Properties.Width, m_Properties.Height, m_Properties.ScaleX, m_Properties.ScaleY, m_Flip);

	GetRigidBody()->GetCollider()->DrawDebugCollider();
}

void Player::Clean(){
	TextureManager::GetInstance()->Drop(m_TextureID);
}

void Player::Update(float dt){

	WalkMovement(dt);
	JumpMovement(dt);
	AnimationState();

	m_Origin->X = m_Transform.X;// + playerConfig.m_PlayerWidth  / 2;
	m_Origin->Y = m_Transform.Y;// + playerConfig.m_PlayerHeight / 2;

	m_Animation->Update(dt);
}

Transform Player::GetBodyCenterPosition() {
	Transform centerTransform;

	// Adjust to ignore the non-body part of the image
	constexpr float imageTopMargin = 35.0f;

	// Calculates the X and Y position of the center of the body
	centerTransform.X = m_Transform.X + (playerConfig.m_PlayerWidth / 2.0f);
	centerTransform.Y = m_Transform.Y + (playerConfig.m_PlayerHeight / 2.0f) + imageTopMargin;

	return centerTransform;
}

void Player::OnTakeDamage(float damage){
	HandlePlayerDeath();
}

void Player::WalkMovement(float dt) {
	if (m_IsDead) return;

	UpdatePlayerDirection();
	ApplyWalkingForce(dt);
	UpdatePlayerPositionX(dt);
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
	GetRigidBody()->SetVelocityX(WALK_VELOCITY * playerDirection);
	GetRigidBody()->Update(dt);
}

void Player::UpdatePlayerPositionX(float dt) {
	m_Transform.X += GetRigidBody()->GetDeltaPosition().X;
	GetRigidBody()->GetCollider()->SetPositionX(GetBodyCenterPosition().X);
	/*std::cout << "pos:" << m_Transform.X << std::endl;*/
}

void Player::JumpMovement(float dt) {
	HandleJumpInput();
	HandleFallingState();
	UpdatePlayerPositionY(dt);
}

void Player::HandleJumpInput() {
	if (m_IsDead) return;

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
		GetRigidBody()->SetVelocityY(UPWARD * JUMP_VELOCITY);
		GetRigidBody()->GetCollider()->SetCollisionResponse(WorldFloor, OVERLAP);
	}
}

void Player::HandleFallingState() {
	if (GetRigidBody()->GetVelocity().Y > 0 && !m_IsGrounded) {
		GetRigidBody()->GetCollider()->SetCollisionResponse(WorldFloor, BLOCK);
		m_IsFalling = true;
		m_IsJumping = false;
	}
	else{
		if (GetRigidBody()->GetVelocity().Y == 0) {
			m_IsGrounded = true;
			m_IsJumping = false;
			m_UsedDoubleJump = false;
		}
		m_IsFalling = false;
	}
}

void Player::UpdatePlayerPositionY(float dt) {
	GetRigidBody()->Update(dt);
	m_Transform.Y += GetRigidBody()->GetDeltaPosition().Y;
	GetRigidBody()->GetCollider()->SetPositionY(GetBodyCenterPosition().Y);
}

bool Player::IsSpacePressed() const {
	return InputHandler::GetInstance()->GetKeyPressed(SDL_SCANCODE_SPACE);
}

void Player::AnimationState(){

	m_Animation->SetProps("Player_Walk", playerConfig.m_PlayerWalkSpriteRow, playerConfig.m_PlayerWalkFrameCount, playerConfig.m_PlayerWalkAnimSpeed);
	/*m_SpriteSheetWidth = 240;
	m_SpriteSheetHeight = 207;*/

	if (m_IsDead) {
		m_Animation->SetProps("Player_Jump", 1, 2, 180);
	}

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

void Player::HandlePlayerDeath() {
	m_IsDead = true;
	GetRigidBody()->SetVelocityX(0);
	if (m_IsJumping) {
		GetRigidBody()->SetVelocityY(GetRigidBody()->GetVelocity().Y/2);
	}
}

void Player::OnCollision(std::shared_ptr<GameObject> target) {
	
}