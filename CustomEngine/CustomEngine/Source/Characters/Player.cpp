#include "Player.h"
#include "../Graphics/TextureManager.h"
//#include "SDL.h"

Player::Player(Properties* props) : Character(props)
{
	m_Animation = new Animation();
	m_Animation->SetProps(m_TextureID, 1, 2, 80, SDL_FLIP_HORIZONTAL);
}

void Player::Draw()
{
	m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);
}

void Player::Clean()
{
	TextureManager::GetInstance()->Clean();
}

void Player::Update(float dt)
{
	m_Animation->Update();
}
