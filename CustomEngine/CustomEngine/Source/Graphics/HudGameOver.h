#pragma once
#include "Hud.h"
#include "../GameUI/GameOver/W_TopHighScores.h"
#include "../GameUI/GameOver/W_NicknameInput.h"
//#include "../States/GameOver.h"

class GameOver;

class HudGameOver : public Hud {
public:
    HudGameOver(SDL_Renderer* renderer, GameOver* gameOverInstance);
    void Render(SDL_Renderer* renderer) override;
    void Init(SDL_Renderer* renderer, GameOver* gameOverInstance);
    void OpenHighScoreScreen();
    void LoadTextures() override;

private:
    W_NicknameInput* nicknameWidget;
};

