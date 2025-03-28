#include "W_GameOverBase.h"
#include "../../States/GameStateManager.h"

void W_GameOverBase::Init() {
    // Adds Game Over background image
    hudBasePanel.textureID = "GameOver_Base_UI";
    hudBasePanel.posX = 70;
    hudBasePanel.posY = 20;
    imagePanels.push_back({ hudBasePanel.textureID, hudBasePanel.posX, hudBasePanel.posY });

    // Adds a restart button
    buttonList.push_back(std::make_unique<Button>(hudBasePanel.posX + 255, hudBasePanel.posY + 550, 115, 115, SDL_Color{ 0, 255, 0, 255 }, [this]() {
        if (gameOverInstance) gameOverInstance->StartGame();
        }));

    // Adds a button to open High Scores
    buttonList.push_back(std::make_unique<Button>(hudBasePanel.posX + 420, hudBasePanel.posY + 550, 115, 115, SDL_Color{ 0, 255, 0, 255 }, [this]() {
        if (gameOverInstance) gameOverInstance->GetHud().OpenHighScoreScreen();
        }));

    // Adds the score text box

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
    scoreBoxTextRect.w = scorePanel.width - boxWidthMargin;
    scoreBoxTextRect.h = scorePanel.height - boxHeightMargin;

    SDL_Color textColor = { 255, 255, 255, 255 };
    std::string scoreText = std::to_string(GameMode::GetInstance()->GetScore());

    textBoxes.push_back(std::make_unique<TextBox>(m_Renderer, scoreBoxTextRect, textColor, 200, scoreText));
}

