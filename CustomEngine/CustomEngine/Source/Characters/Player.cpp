#include "Player.h"
#include "../Graphics/TextureManager.h"
#include "SDL.h"

Player::Player(Properties* props) : Character(props)
{
	m_Row = 1;
	m_FrameCount = 2;
	m_AnimSpeed = 90;
}

void Player::Draw()
{
	TextureManager::GetInstance()->DrawFrame(m_TextureID, m_Transform->X, m_Transform->Y, (m_Width/ m_FrameCount), (m_Height/ m_Row), m_Row, m_Frame);
}

void Player::Clean()
{
	TextureManager::GetInstance()->Clean();
}

void Player::Update(float dt)
{
	m_Frame = (SDL_GetTicks() / m_AnimSpeed) % m_FrameCount;
}
