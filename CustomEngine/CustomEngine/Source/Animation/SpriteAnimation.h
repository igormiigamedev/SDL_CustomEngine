#pragma once

#include <SDL.h>
#include <map>
#include <string>
#include "Animation.h"

struct SpriteSheet {
	int Speed;
	int FrameCount;
	int RowCount;
	int ColCount;
	int Width, Height;
	std::string TextureID;
};

class SpriteAnimation : public Animation {

	public:
		SpriteAnimation(bool repeat = true);

		virtual void Update(float dt);
		void Draw(float x, float y, float scaleX = 1.0f, float scaleY = 1.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void Parse();
		void SetAnimation(const std::string& textureID, int animSpeed = 0);

		bool IsValidTexture(const std::string& textureID);
		void HandleInvalidTexture(const std::string& textureID);

		inline int GetCurrentSpriteHeight() { return m_CurrentSpriteSheet.Height / m_CurrentSpriteSheet.RowCount; }
		inline int GetCurrentSpriteWidth() { return m_CurrentSpriteSheet.Width / m_CurrentSpriteSheet.ColCount; }

	private:
		int m_SpriteFrame;
		SpriteSheet m_CurrentSpriteSheet;
		std::map<std::string, SpriteSheet> m_SpriteSheetMap;

};
