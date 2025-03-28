#include "W_TopHighScores.h"

void W_TopHighScores::Init(){
    // Adds the High Scores panel
    highScorePanel.textureID = "GameOver_Top_Highscores_UI";
    highScorePanel.posX = 70;
    highScorePanel.posY = 20;
    highScorePanel.width = TextureManager::GetInstance()->GetTextureSize(highScorePanel.textureID).width;
    highScorePanel.height = TextureManager::GetInstance()->GetTextureSize(highScorePanel.textureID).height;

    imagePanels.push_back({ highScorePanel.textureID, highScorePanel.posX, highScorePanel.posY });

    // Add a close button
    buttonList.push_back(std::make_unique<Button>(200, 100, 115, 115, SDL_Color{ 0, 255, 0, 255 }, [this]() {
        Hide();
        }));

    // Add Top 10 High Scores
    SDL_Color textColor = { 255, 255, 255, 255 };
    int textBoxHeight = 60;
    int textBoxWidth = 120;
    int yTextOffset = 100;

    for (size_t i = 0; i < GameMode::GetInstance()->GetTopHighScores().size(); ++i) {
        std::string highScoreText = std::to_string(i + 1) + ". " + std::to_string(GameMode::GetInstance()->GetTopHighScores()[i]);
        SDL_Rect hsRect = { highScorePanel.posX + (highScorePanel.width / 2 - textBoxWidth / 2), static_cast<int>(yTextOffset + (i * textBoxHeight)), textBoxWidth, textBoxHeight };

        textBoxes.push_back(std::make_unique<TextBox>(m_Renderer, hsRect, textColor, 200, highScoreText));
    }
}