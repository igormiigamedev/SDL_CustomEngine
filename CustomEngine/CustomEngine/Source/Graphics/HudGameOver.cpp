#include "HudGameOver.h"
#include "TextureManager.h"

HudGameOver::HudGameOver(SDL_Renderer* renderer) {
	LoadTextures();
	Init(renderer);
}

void HudGameOver::Render(SDL_Renderer* renderer) {
	Hud::Render(renderer);

	RenderGameOverBaseHud(renderer);

	if (highScoreScreenIsOpen) {
		RenderTopHighScoresHUD(renderer);
	}
}

void HudGameOver::Init(SDL_Renderer* renderer) {
	SDL_Color textColor = { 255, 255, 255, 255 };

	// Base Hud Panel
	hudBasePanel.textureID = "GameOver_Base_UI";
	hudBasePanel.posX = 70;
	hudBasePanel.posY = 20;

	TextureManager::GetInstance()->Draw(hudBasePanel.textureID, hudBasePanel.posX, hudBasePanel.posY, 1.0f, 1.0f);

	//Score Box Panel Infos
	scorePanel.posX = 230 + hudBasePanel.posX;
	scorePanel.posY = 350 + hudBasePanel.posY;
	scorePanel.width = 350;
	scorePanel.height = 106;

	//Score Box Text Infos
	int boxWidthMargin = 10;
	int boxHeightMargin = 20;
	SDL_Rect scoreBoxTextRect;
	scoreBoxTextRect.x = scorePanel.posX;
	scoreBoxTextRect.y = scorePanel.posY + boxHeightMargin / 2;
	scoreBoxTextRect.w = scorePanel.width  - boxWidthMargin;
	scoreBoxTextRect.h = scorePanel.height - boxHeightMargin;

	std::string scoreText = std::to_string(GameMode::GetInstance()->GetScore());

	scoreTextBox = std::make_unique<TextBox>(renderer, scoreBoxTextRect, textColor, 200, scoreText);

	//HighScore Screen
	highScorePanel.textureID = "GameOver_Top_Highscores_UI";
	highScorePanel.posX = 70;
	highScorePanel.posY = 20;
	highScorePanel.width = TextureManager::GetInstance()->GetTextureSize(highScorePanel.textureID).width;
	highScorePanel.height = TextureManager::GetInstance()->GetTextureSize(highScorePanel.textureID).height;

	TextureManager::GetInstance()->Draw(highScorePanel.textureID, highScorePanel.posX, highScorePanel.posY, 1.0f, 1.0f);

	int textBoxHeight = 60; // height of the rectangle where the text will be drawn
	int textBoxWidth = 120; // width of the rectangle where the text will be drawn

	// Show top 10 highscores
	int yTextOffset = 100;
	for (size_t i = 0; i < GameMode::GetInstance()->GetTopHighScores().size(); ++i) {
		std::string highScoreText = std::to_string(i + 1) + ". " + std::to_string(GameMode::GetInstance()->GetTopHighScores()[i]);
		SDL_Rect hsRect = { highScorePanel.posX + (highScorePanel.width / 2 - textBoxWidth / 2), static_cast<int>(yTextOffset + (i * textBoxHeight)), textBoxWidth, textBoxHeight };

		std::unique_ptr<TextBox> textTeste = std::make_unique<TextBox>(renderer, hsRect, textColor, 200, highScoreText);
		topHighScores.push_back(std::move(textTeste));
	}
}

void HudGameOver::LoadTextures() {
	TextureManager::GetInstance()->ParseTextures("Assets/HudTextures_GameOver.xml");
}

void HudGameOver::RenderTopHighScoresHUD(SDL_Renderer* renderer) {
	TextureManager::GetInstance()->Draw(highScorePanel.textureID, highScorePanel.posX, highScorePanel.posY, 1.0f, 1.0f);

	for (size_t i = 0; i < topHighScores.size(); i++) {
		topHighScores[i]->Render(renderer);
	}
}

void HudGameOver::RenderGameOverBaseHud(SDL_Renderer* renderer) {
	TextureManager::GetInstance()->Draw(hudBasePanel.textureID, hudBasePanel.posX, hudBasePanel.posY, 1.0f, 1.0f);

	scoreTextBox->Render(renderer);

}