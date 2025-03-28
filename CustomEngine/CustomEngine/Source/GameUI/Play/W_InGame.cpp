#include "W_InGame.h"
#include "../../GameMode/GameMode.h"
#include "../../Core/Engine.h"

void W_InGame::Init() {
	SDL_Color textColor = { 255, 255, 255, 255 };

	// Score Panel
	scorePanel.textureID = "InGame_ScorePanel_UI";
	scorePanel.posX = 70;
	scorePanel.posY = 20;

	imagePanels.push_back({ scorePanel.textureID, scorePanel.posX, scorePanel.posY });

	// Score Text
	int boxWidthMargin = 10;
	int boxHeightMargin = 20;
	int cornerImageWidth = 70;
	SDL_Rect scoreBoxTextRect;
	scoreBoxTextRect.x = scorePanel.posX + cornerImageWidth;
	scoreBoxTextRect.y = scorePanel.posY + boxHeightMargin / 2;
	scoreBoxTextRect.w = TextureManager::GetInstance()->GetTextureSize(scorePanel.textureID).width - cornerImageWidth - boxWidthMargin;
	scoreBoxTextRect.h = TextureManager::GetInstance()->GetTextureSize(scorePanel.textureID).height - boxHeightMargin;

	std::string scoreText = std::to_string(GameMode::GetInstance()->GetScore());

	std::unique_ptr<TextBox> scoreTextBox = std::make_unique<TextBox>(m_Renderer, scoreBoxTextRect, textColor, 200, scoreText);
	scoreTextBoxPtr = scoreTextBox.get();
	textBoxes.push_back(std::move(scoreTextBox));

	// HighScore
	std::string highScoreText = " High Score: " + std::to_string(GameMode::GetInstance()->GetTopHighScores().empty() ? 0 : GameMode::GetInstance()->GetTopHighScores()[0]);

	int textHeight = 70; // height of the rectangle where the text will be drawn
	int textWidth = 350; // width of the rectangle where the text will be drawn

	SDL_Rect textRect;
	textRect.w = textWidth;
	textRect.h = textHeight;
	textRect.x = SCREEN_WIDTH / 2 - textRect.w / 2;
	textRect.y = 20;

	textBoxes.push_back(std::make_unique<TextBox>(m_Renderer, textRect, textColor, 200, highScoreText));
}

void W_InGame::SetScoreText(std::string newScoreText) {
	scoreTextBoxPtr->SetText(newScoreText);
}