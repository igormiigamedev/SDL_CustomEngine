#include "SpriteAnimation.h"
#include "../Graphics/TextureManager.h"

SpriteAnimation::SpriteAnimation(bool repeat) :Animation(repeat) {}

void SpriteAnimation::Update(float dt){
	m_SpriteFrame = (SDL_GetTicks() / m_AnimSpeed) % m_FrameCount;
}

void SpriteAnimation::Draw(float x, float y, int spriteWidth, int spriteHeight, float imageScalling, SDL_RendererFlip flip){
	TextureManager::GetInstance()->DrawFrame(m_TextureID, x, y, (spriteWidth/ m_FrameCount), (spriteHeight/ m_SpriteRow), m_SpriteRow, m_SpriteFrame, flip, imageScalling);
}

void SpriteAnimation::SetProps(std::string textureID, int spriteRow, int frameCount, int animSpeed){
	m_TextureID = textureID;
	m_SpriteRow = spriteRow;
	m_FrameCount = frameCount;
	m_AnimSpeed = animSpeed;
}