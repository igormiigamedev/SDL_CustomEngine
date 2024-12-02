#include "Enemy.h"
#include "../Factory/ObjectFactory.h"

static RegisterObject<Enemy> registerObject(GameObjectType::ENEMY);

Enemy::Enemy(const Properties& props, Transform transform) : Character(props, transform){
	RegisterCollisionCallback();

	GetRigidBody()->SetGravity(360.0f);
	GetRigidBody()->GetCollider()->SetBuffer(0, 0, 0, 0);

	m_Animation = new SequenceAnimation(false);
	m_Animation->Parse("Assets/GameAnimations.xml");
	m_Animation->SetCurrentSequence("spikeMan_Walk");

	enemyConfig.m_EnemyWidth = m_Properties.ScaleX * (m_Properties.Width );
	enemyConfig.m_EnemyHeight = m_Properties.ScaleY * (m_Properties.Height );
}

void Enemy::Draw(){
	m_Animation->DrawFrame(m_Transform.X, m_Transform.Y, m_Properties.ScaleX, m_Properties.ScaleY, m_Flip);
	GetRigidBody()->GetCollider()->SetProperties(m_Transform.X, m_Transform.Y, enemyConfig.m_EnemyWidth, (enemyConfig.m_EnemyHeight));
	GetRigidBody()->GetCollider()->DrawDebugCollider();
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

void Enemy::OnTakeDamage(float damage){
}

void Enemy::WalkMovement(float dt) {

	UpdateCharacterDirection();
	ApplyWalkingForce(dt);
	UpdateCharacterPositionX(dt);

}

void Enemy::UpdateCharacterDirection() {
	if (m_Transform.X >= (SCREEN_WIDTH - 120)) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
		characterDirection = BACKWARD;
		m_Flip = SDL_FLIP_HORIZONTAL;
	}
	else if (m_Transform.X <= 0) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
		characterDirection = FORWARD;
		m_Flip = SDL_FLIP_NONE;
	}
}

void Enemy::ApplyWalkingForce(float dt) {
	GetRigidBody()->SetVelocityX(WALK_VELOCITY * characterDirection);
	GetRigidBody()->Update(dt);
}

void Enemy::UpdateCharacterPositionX(float dt) {
	m_Transform.X += GetRigidBody()->GetDeltaPosition().X;
	GetRigidBody()->GetCollider()->SetPositionX(m_Transform.X);
}

void Enemy::UpdateCharacterPositionY(float dt) {
	/*m_RigidBody->Update(dt);*/
	m_Transform.Y += GetRigidBody()->GetDeltaPosition().Y;
	GetRigidBody()->GetCollider()->SetPositionY(m_Transform.Y);
}

void Enemy::OnCollision(std::shared_ptr<GameObject> target) {
	if (target) {
		auto character = std::dynamic_pointer_cast<Character>(target);
		if (character && character->GetType() == GameObjectType::PLAYER) {
			/*std::cout << "Enemy colidiu com Player!" << std::endl;*/
			character->ApplyDamage(1);
		}
	}
}