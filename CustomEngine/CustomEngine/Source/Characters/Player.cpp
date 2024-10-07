#include "Player.h"
#include "../Graphics/TextureManager.h"
#include "../Inputs/InputHandler.h"
#include "SDL.h"
#include "../Core/Engine.h" // TODO - Remove when update Player Flip logic

Player::Player(Properties* props) : Character(props){
	m_RigidBody = new RigidBody();
	m_Animation = new Animation();
	m_Animation->SetProps(m_TextureID, 1, 2, 100);
}

void Player::Draw(){
	m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);
}

void Player::Clean(){
	TextureManager::GetInstance()->Clean();
}

void Player::Update(float dt){

	/*m_Animation->SetProps("Player_Walk", 1, 2, 100);*/ //Player_Walk
	
	// TODO - Jump Handler
	/*if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) {
		m_Animation->SetProps("Player_Jump", 1, 2, 180);
		m_RigidBody->ApplyForceY(55*UPWARD);
	}
	else {
		m_Animation->SetProps("Player_Walk", 1, 2, 100);
		m_RigidBody->UnSetForceY();
	}*/
	if (m_Transform->X >= (SCREEN_WIDTH - 120)) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
		playerDirection = BACKWARD;
		m_Animation->SetProps("Player_Walk", 1, 2, 100, SDL_FLIP_HORIZONTAL);
	}
	else if (m_Transform->X <= 0) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
		playerDirection = FORWARD;
		m_Animation->SetProps("Player_Walk", 1, 2, 100);
	}

	m_RigidBody->ApplyForceX(6 * playerDirection);

	m_RigidBody->Update(0.6); //TODO (delta time) - dt
	m_Transform->X += m_RigidBody->GetPosition().X;

	// TODO - Jump Handler
	//if ((m_Transform->Y + m_RigidBody->GetPosition().Y) < (SCREEN_HEIGHT - 280)) { //TODO (scenery collision) - Check if you collided with an object with the tag "wall"
	//	m_Transform->Y += m_RigidBody->GetPosition().Y;
	//}

	m_Animation->Update();
}
