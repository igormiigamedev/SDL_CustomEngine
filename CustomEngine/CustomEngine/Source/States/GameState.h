#pragma once
#include "SDL.h"
#include <SDL_ttf.h>

class GameState{

	public:
		virtual bool Init();
		virtual bool Exit() = 0;

		virtual void Update() = 0;
		virtual void Render() = 0;

	protected:
		SDL_Renderer* m_Ctxt;

		virtual void SetHud() = 0; // Each state will define its appropriate HUD
};

