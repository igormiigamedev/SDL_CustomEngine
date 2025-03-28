#include "HudGameOver.h"
#include "TextureManager.h"
#include "../GameUI/GameOver/W_GameOverBase.h"

HudGameOver::HudGameOver(SDL_Renderer* renderer, GameOver* gameOverInstance) {
	LoadTextures();
	Init(renderer, gameOverInstance);
}

void HudGameOver::Render(SDL_Renderer* renderer) {
	Hud::Render(renderer);
}

void HudGameOver::Init(SDL_Renderer* renderer, GameOver* gameOverInstance) {
	CreateWidget<W_GameOverBase>(renderer, gameOverInstance);
	topHighScoresWidget = CreateWidget<W_TopHighScores>(renderer);
}

void HudGameOver::OpenHighScoreScreen() {
	if (topHighScoresWidget) {
		topHighScoresWidget->Show();
	}
	return;
}

void HudGameOver::LoadTextures() {
	TextureManager::GetInstance()->ParseTextures("Assets/HudTextures_GameOver.xml");
}