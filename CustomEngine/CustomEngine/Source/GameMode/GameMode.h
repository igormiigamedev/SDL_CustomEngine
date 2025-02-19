#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class GameMode {
private:
    int playerScore;
    int highScore;
    static GameMode* instance; // Pointer to the Singleton

    // Private constructor to avoid multiple instances
    GameMode() : playerScore(0), highScore(0) {}

public:
    static GameMode* GetInstance(); // Method to get the single instance

    void IncreaseScore(int amount);
    void SaveHighScore();
    void ResetScore();
    void RenderHUD(SDL_Renderer* renderer) const;

    int GetScore() const;
    int GetHighScore() const;
};
