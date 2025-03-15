#include "HudPlay.h"
#include "../Text/TextManager.h"
#include "TextureManager.h"
#include "../Core/Engine.h"

HudPlay::HudPlay(SDL_Renderer* renderer) {
	LoadTextures();
	Init(renderer);
}

void HudPlay::Render(SDL_Renderer* renderer) {
	Hud::Render(renderer);

    // Renders the base of the in-game HUD
	TextureManager::GetInstance()->Draw(scorePanel.textureID, scorePanel.posX, scorePanel.posY, 1.0f, 1.0f);

    RenderScore(renderer);
}

void HudPlay::LoadTextures() {
    TextureManager::GetInstance()->ParseTextures("Assets/HudTextures_Play.xml");
}

void HudPlay::Init(SDL_Renderer* renderer){
	SDL_Color textColor = { 255, 255, 255, 255 };

	// Score Panel
	scorePanel.textureID = "InGame_ScorePanel_UI";
	scorePanel.posX = 70;
	scorePanel.posY = 20;
	
	TextureManager::GetInstance()->Draw(scorePanel.textureID, scorePanel.posX, scorePanel.posY, 1.0f, 1.0f);

	// Score Text
	int boxWidthMargin = 10;
	int boxHeightMargin = 20;
	int cornerImageWidth = 70;
	SDL_Rect scoreBoxTextRect;
	scoreBoxTextRect.x = scorePanel.posX + cornerImageWidth;
	scoreBoxTextRect.y = scorePanel.posY + boxHeightMargin/2;
	scoreBoxTextRect.w = TextureManager::GetInstance()->GetTextureSize(scorePanel.textureID).width - cornerImageWidth - boxWidthMargin;
	scoreBoxTextRect.h = TextureManager::GetInstance()->GetTextureSize(scorePanel.textureID).height - boxHeightMargin;

	std::string scoreText = std::to_string(GameMode::GetInstance()->GetScore());

	scoreTextBox = std::make_unique<TextBox>(renderer, scoreBoxTextRect, textColor, 200, scoreText);

	// HighScore
	std::string highScoreText = " High Score: " + std::to_string(GameMode::GetInstance()->GetTopHighScores().empty() ? 0 : GameMode::GetInstance()->GetTopHighScores()[0]);

	int textHeight = 70; // height of the rectangle where the text will be drawn
	int textWidth = 350; // width of the rectangle where the text will be drawn

	SDL_Rect textRect;
	textRect.w = textWidth;
	textRect.h = textHeight;
	textRect.x = SCREEN_WIDTH / 2 - textRect.w / 2;
	textRect.y = 20;

	highScoreTextBox = std::make_unique<TextBox>(renderer, textRect, textColor, 200, highScoreText);
}

void HudPlay::RenderScore(SDL_Renderer* renderer) {
	scoreTextBox->Render(renderer);
	highScoreTextBox->Render(renderer);
}

