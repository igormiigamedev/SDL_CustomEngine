#include "Enemy.h"

Enemy::Enemy(Properties* props) : Character(props){
	m_RigidBody = new RigidBody();
	m_RigidBody->SetGravity(360.0f);
	m_RigidBody->GetCollider()->SetBuffer(0, 0, 0, 0);

	m_Animation = new SequenceAnimation(false);
	m_Animation->Parse("Assets/GameAnimations.xml");
	m_Animation->SetCurrentSequence("spikeMan_Walk");

	enemyConfig.m_EnemyWidth = m_ImageScalling * (m_SpriteSheetWidth );
	enemyConfig.m_EnemyHeight = m_ImageScalling * (m_SpriteSheetHeight );
}

void Enemy::Draw(){
	m_Animation->DrawFrame(m_Transform->X, m_Transform->Y, m_ImageScalling, m_ImageScalling, m_Flip);
	m_RigidBody->GetCollider()->SetProperties(m_Transform->X, m_Transform->Y, enemyConfig.m_EnemyWidth, (enemyConfig.m_EnemyHeight - 30));
	m_RigidBody->GetCollider()->DrawDebugCollider();
}

void Enemy::Clean(){
	/*TextureManager::GetInstance()->Drop(m_TextureID);*/
}

void Enemy::Update(float dt){
	WalkMovement(dt);

	UpdateCharacterPositionY(dt);

	/*AnimationState();*/

	m_Animation->Update(dt);

	if (m_Animation->IsEnded()) {
		m_Animation->SetRepeat(true);
		m_Animation->SetCurrentSequence("spikeMan_Walk");
	}
}

void Enemy::WalkMovement(float dt) {

	UpdateCharacterDirection();
	ApplyWalkingForce(dt);
	UpdateCharacterPositionX(dt);

}

void Enemy::UpdateCharacterDirection() {
	if (m_Transform->X >= (SCREEN_WIDTH - 120)) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
		characterDirection = BACKWARD;
		m_Flip = SDL_FLIP_HORIZONTAL;
	}
	else if (m_Transform->X <= 0) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
		characterDirection = FORWARD;
		m_Flip = SDL_FLIP_NONE;
	}
}

void Enemy::ApplyWalkingForce(float dt) {
	m_RigidBody->SetVelocityX(WALK_VELOCITY * characterDirection);
	m_RigidBody->Update(dt);
}

void Enemy::UpdateCharacterPositionX(float dt) {
	m_Transform->X += m_RigidBody->GetDeltaPosition().X;
	m_RigidBody->GetCollider()->SetPositionX(m_Transform->X);
}

void Enemy::UpdateCharacterPositionY(float dt) {
	/*m_RigidBody->Update(dt);*/
	m_Transform->Y += m_RigidBody->GetDeltaPosition().Y;
	m_RigidBody->GetCollider()->SetPositionY(m_Transform->Y);
}