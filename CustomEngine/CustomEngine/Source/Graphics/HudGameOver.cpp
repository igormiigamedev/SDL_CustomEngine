#include "HudGameOver.h"
#include "TextureManager.h"
#include "../GameUI/GameOver/W_GameOverBase.h"

HudGameOver::HudGameOver(SDL_Renderer* renderer, GameOver* gameOverInstance) {
	LoadTextures();
	Init(renderer, gameOverInstance);
}

void HudGameOver::Render(SDL_Renderer* renderer) {
	Hud::Render(renderer);
	nicknameWidget->Render(); 
}

void HudGameOver::Init(SDL_Renderer* renderer, GameOver* gameOverInstance) {
	CreateWidget<W_GameOverBase>(renderer, gameOverInstance);
	CreateWidget<W_TopHighScores>(renderer);
	nicknameWidget = CreateWidget<W_NicknameInput>(renderer);
	nicknameWidget->Init();
}

void HudGameOver::OpenHighScoreScreen() {
	if (W_TopHighScores* highScoreWidget = GetWidgetOfClass<W_TopHighScores>()) {
		highScoreWidget->Show();
	}
}

void HudGameOver::LoadTextures() {
	TextureManager::GetInstance()->ParseTextures("Assets/HudTextures_GameOver.xml");
}