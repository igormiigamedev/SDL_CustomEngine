#include "GameMode.h"
#include "../Core/Engine.h"
#include <algorithm>
#include <fstream>
#include <algorithm>
#include "../Text/TextManager.h"

const std::string HIGHSCORE_FILE = "highscores.dat";

GameMode* GameMode::instance = nullptr;

GameMode* GameMode::GetInstance() {
    if (!instance) {
        instance = new GameMode();
        instance->LoadHighScores(); 
    }
    return instance;
}

void GameMode::IncreaseScore(int amount) {
    playerScore += amount;
}

void GameMode::SaveHighScore() {
    // Adds the current score to the highscores list
    highScores.push_back(playerScore);

    // Sort in descending order and keep only the top 10
    std::sort(highScores.rbegin(), highScores.rend());
    if (highScores.size() > NUMBER_OF_HIGHSCORES) {
        highScores.resize(NUMBER_OF_HIGHSCORES);
    }

    // Saves highscores to file
    std::ofstream file(HIGHSCORE_FILE, std::ios::binary | std::ios::trunc);
    if (file.is_open()) {
        for (int score : highScores) {
            file.write(reinterpret_cast<char*>(&score), sizeof(score));
        }
        file.close();
    }
}

void GameMode::LoadHighScores() {
    highScores.clear();

    std::ifstream file(HIGHSCORE_FILE, std::ios::binary);
    if (file.is_open()) {
        int score;
        while (file.read(reinterpret_cast<char*>(&score), sizeof(score))) {
            highScores.push_back(score);
        }
        file.close();
    }

    // Ensures that we only have a maximum of 10 scores stored
    std::sort(highScores.rbegin(), highScores.rend());
    if (highScores.size() > NUMBER_OF_HIGHSCORES) {
        highScores.resize(NUMBER_OF_HIGHSCORES);
    }
}

void GameMode::ResetScore() {
    playerScore = 0;
}

void GameMode::RenderScoreHUDInGame(SDL_Renderer* renderer) const {
    int textHeight = 70; // height of the rectangle where the text will be drawn
    int textWidth = 350; // width of the rectangle where the text will be drawn
    int ptsize = 0;
    if (textHeight < textWidth) {
        ptsize = textHeight * 1.5;
    }
    else {
        ptsize = textWidth * 1.5;
    }

    std::string scoreText = "Score: " + std::to_string(playerScore) + "  High Score: " + std::to_string(highScores.empty() ? 0 : highScores[0]);

    // Set position and size of text on screen
    SDL_Rect textRect;
    textRect.w = textWidth;
    textRect.h = textHeight;
    textRect.x = SCREEN_WIDTH / 2 - textRect.w / 2;
    textRect.y = 20;

    SDL_Color textColor = { 255, 255, 255, 255 };

    TextManager::GetInstance()->RenderText(renderer, scoreText, textRect, textColor, ptsize);
}

int GameMode::GetScore() const {
    return playerScore;
}

int GameMode::GetHighScore() const {
    return highScores.empty() ? 0 : highScores[0];
}

std::vector<int> GameMode::GetTopHighScores() const
{
    return highScores;
}