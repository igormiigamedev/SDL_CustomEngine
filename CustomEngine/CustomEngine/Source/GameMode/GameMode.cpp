#include "GameMode.h"
#include "../Core/Engine.h"
#include <algorithm>
#include <fstream>
#include <algorithm>
#include "../Text/TextManager.h"
#include "../Graphics/HudPlay.h"
#include "../States/GameStateManager.h"

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

    HudPlay* hud = dynamic_cast<HudPlay*>(GameStateManager::GetInstance()->GetCurrentHud());
    if (hud) {
        hud->SetScoreText(std::to_string(playerScore));
    }
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