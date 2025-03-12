#pragma once
#include <SDL_timer.h>
#include <SDL_render.h>
#include <iostream>

class Hud
{
public:
	static Hud* GetInstance(); // Method to get the single instance

	bool PlayFadeOut(int fadeDuration);

	void RenderFadeOut(SDL_Renderer* renderer) const;

	void RenderScoreHUDInGame(SDL_Renderer* renderer) const;

	void RenderTopHighScoresHUD(SDL_Renderer* renderer) const;

	void RenderGameOverBaseHud( SDL_Renderer* renderer);

	void LoadTextures();

private:
	static Hud* instance;

	Uint32 fadeInitTime = 0; // Time the fade init
	bool waitingForFadeOut = false;
	int fadeAlpha = 0;  // Fade opacity level (0 = invisible, 255 = completely black)
};

