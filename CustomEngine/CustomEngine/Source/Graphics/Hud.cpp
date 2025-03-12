#include "Hud.h"
#include "../Core/Engine.h"
#include "../GameMode/GameMode.h"
#include "../Text/TextManager.h"
#include "TextureManager.h"

Hud* Hud::instance = nullptr;

Hud* Hud::GetInstance() {
	if (!instance) {
		instance = new Hud();
	}
	return instance;
}

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
		std::cout << "fadeAlpha: " << fadeAlpha << std::endl;
		SDL_Rect fadeRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

		// Enables blending to allow transparency
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, fadeAlpha);  // Black color with variable alpha

		SDL_RenderFillRect(renderer, &fadeRect);

		// Restores the default blending mode (optional)
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	}
}

void Hud::RenderScoreHUDInGame(SDL_Renderer* renderer) const {
	int textHeight = 70; // height of the rectangle where the text will be drawn
	int textWidth = 350; // width of the rectangle where the text will be drawn
	int ptsize = 0;
	if (textHeight < textWidth) {
		ptsize = textHeight * 1.5;
	}
	else {
		ptsize = textWidth * 1.5;
	}

	std::string scoreText = "Score: " + std::to_string(GameMode::GetInstance()->GetScore()) + "  High Score: " + std::to_string(GameMode::GetInstance()->GetTopHighScores().empty() ? 0 : GameMode::GetInstance()->GetTopHighScores()[0]);

	// Set position and size of text on screen
	SDL_Rect textRect;
	textRect.w = textWidth;
	textRect.h = textHeight;
	textRect.x = SCREEN_WIDTH / 2 - textRect.w / 2;
	textRect.y = 20;

	SDL_Color textColor = { 255, 255, 255, 255 };

	TextManager::GetInstance()->RenderText(renderer, scoreText, textRect, textColor, ptsize);
}

void Hud::RenderTopHighScoresHUD(SDL_Renderer* renderer) const {
	std::string textureID = "GameOver_Top_Highscores_UI";

	int baseHudPosX = 70;
	int baseHudPosY = 20;

	TextureManager::GetInstance()->Draw(textureID, baseHudPosX, baseHudPosY, 1.0f, 1.0f);

	int textHeight = 60; // height of the rectangle where the text will be drawn
	int textWidth = 120; // width of the rectangle where the text will be drawn
	int ptsize = 0;
	if (textHeight < textWidth) {
		ptsize = textHeight * 1.5;
	}
	else {
		ptsize = textWidth * 1.5;
	}

	// Load font
	/*TTF_Font* font = TTF_OpenFont("Assets/Fonts/flying_bubble/Flying Bubble.otf", ptsize);
	if (!font) {
		std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
		return;
	}*/

	SDL_Color textColor = { 255, 255, 255, 255 };

	// Show top 10 highscores
	int yTextOffset = 100;
	for (size_t i = 0; i < GameMode::GetInstance()->GetTopHighScores().size(); ++i) {
		std::string highScoreText = std::to_string(i + 1) + ". " + std::to_string(GameMode::GetInstance()->GetTopHighScores()[i]);
		SDL_Rect hsRect = { SCREEN_WIDTH / 2 - textWidth / 2, static_cast<int>(yTextOffset + (i * textHeight)), textWidth, textHeight };
		TextManager::GetInstance()->RenderText(renderer, highScoreText, hsRect, textColor, ptsize);
	}

	/*TTF_CloseFont(font);*/
}

void Hud::RenderGameOverBaseHud( SDL_Renderer* renderer) {
	std::string textureID = "GameOver_Base_UI";

	int baseHudPosX = 70;
	int baseHudPosY = 20;

	TextureManager::GetInstance()->Draw(textureID, baseHudPosX, baseHudPosY, 1.0f, 1.0f);

	//Score Box Infos
	SDL_Rect scoreBoxRect;
	scoreBoxRect.x = 230;
	scoreBoxRect.y = 350;
	scoreBoxRect.w = 350;
	scoreBoxRect.h = 106;

	std::string scoreText = std::to_string(GameMode::GetInstance()->GetScore());

	SDL_Rect scoreBoxTransform = { scoreBoxRect.x + baseHudPosX, scoreBoxRect.y + baseHudPosY, scoreBoxRect.w, scoreBoxRect.h };
	SDL_Rect scoreTextTransform = TextManager::GetInstance()->CentralizeTextAtBox(scoreBoxTransform, scoreText);

	TextManager::GetInstance()->RenderText(renderer, scoreText, scoreTextTransform, { 255, 255, 255, 255 }, 100);
}

void Hud::LoadTextures() {
	TextureManager::GetInstance()->ParseTextures("Assets/GameTextures.xml");
}

