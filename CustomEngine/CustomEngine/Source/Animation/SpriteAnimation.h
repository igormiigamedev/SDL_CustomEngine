#pragma once

#include <SDL.h>
#include <string>
#include "Animation.h"

class SpriteAnimation : public Animation {

	public:
		SpriteAnimation(bool repeat = true);

		virtual void Update(float dt);
		void Draw(float x, float y, int spriteWidth, int spriteHeight, float scaleX = 1.0f, float scaleY = 1.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void SetProps(std::string textureID, int spriteRow, int frameCount, int animSpeed);

		/*int GetCurrFrame() { return  m_CurrentFrame; }

		int GetCurrSpriteRow() { return m_SpriteRow; }
		void SetSpriteRow(int row) { m_SpriteRow = row; }
		void IncrSpriteRow() { m_SpriteRow++; }
		void DecrSpriteRow() { m_SpriteRow--; }*/

	private:
		int m_SpriteRow, m_SpriteFrame, m_FrameCount;
		int m_AnimSpeed;
		std::string m_TextureID;

};
