#pragma once
#include <SDL.h>
#include <string>

#ifndef ANIMATION_H
#define ANIMATION_H

class Animation
{
	public:
		Animation();

		void Update();
		void Draw(float x, float y, int spriteWidth, int spriteHeight, float imageScalling, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void SetProps(std::string textureID, int spriteRow, int frameCount, int animSpeed);

	private:
		int m_SpriteRow, m_SpriteFrame, m_FrameCount;
		int m_AnimSpeed;
		std::string m_TextureID;

};

#endif // !ANIMATION_H
