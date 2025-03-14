#pragma once

#include <map>
#include <string>
#include <vector>
#include "SDL.h"
#include "Animation.h"

struct Sequence {
	int Speed;
	int FrameCount;
	int Width, Height;
	std::vector<std::string> TextureIDs;
};

class SequenceAnimation : public Animation{

	public:
		SequenceAnimation(bool repeat = true);

		virtual void Update(float dt);
		void Parse(std::string source);
		void SetCurrentSequence(std::string seqID);
		void SetRepeat(bool repeat) { m_Repeat = repeat; }
		void DrawFrame(float x, float y, float xScale = 1, float yScale = 1, SDL_RendererFlip flip = SDL_FLIP_NONE);

		inline int GetCurrentSpriteHeight() { return m_CurrentSeq.Height; }
		inline int GetCurrentSpriteWidth() { return m_CurrentSeq.Width; }

	private:
		Sequence m_CurrentSeq;
		std::map<std::string, Sequence> m_SeqMap;
};

