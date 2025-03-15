#include "Hud.h"
#include "../Core/Engine.h"
#include "../Text/TextManager.h"
#include "TextureManager.h"

bool Hud::PlayFadeOut(int fadeDuration) {
	if (!waitingForFadeOut) {
		// Save initial time and activate standby
		fadeInitTime = SDL_GetTicks();
		waitingForFadeOut = true;
		fadeAlpha = 0;
	}
	else {
		// Calculates elapsed time
		Uint32 elapsed = SDL_GetTicks() - fadeInitTime;
		float timeLimit = fadeDuration * 1000; //5000 milliseconds = 5 seconds

		// If less than the time limit has passed, we increase the transparency of the fade
		if (elapsed < timeLimit) {
			fadeAlpha = static_cast<int>((elapsed / timeLimit) * 255);  // Maps time -> opacity
		}
		else {
			waitingForFadeOut = false;
			fadeAlpha = 0;
			return true;
		}
	}
	return false;
}

void Hud::RenderFadeOut(SDL_Renderer* renderer) const {
	if (fadeAlpha > 0) {
		SDL_Rect fadeRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

		// Enables blending to allow transparency
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, fadeAlpha);  // Black color with variable alpha

		SDL_RenderFillRect(renderer, &fadeRect);

		// Restores the default blending mode (optional)
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	}
}

