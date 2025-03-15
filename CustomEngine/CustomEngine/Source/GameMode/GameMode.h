#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#define NUMBER_OF_HIGHSCORES 10

class GameMode {
private:
    int playerScore;
    std::vector<int> highScores;
    static GameMode* instance; // Pointer to the Singleton

    // Private constructor to avoid multiple instances
    GameMode() : playerScore(0), highScores(0) {}

public:
    static GameMode* GetInstance(); // Method to get the single instance

    void IncreaseScore(int amount);
    void SaveHighScore();
    void LoadHighScores();
    void ResetScore();

    int GetScore() const;
    int GetHighScore() const;
    std::vector<int> GetTopHighScores() const;
};
