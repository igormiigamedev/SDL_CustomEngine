#pragma once
#include "Hud.h"
#include "../GameUI/Play/W_InGame.h"

class HudPlay : public Hud {
public:
    HudPlay(SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer) override;
    void LoadTextures() override;

    void Init(SDL_Renderer* renderer);
    void SetScoreText(const std::string& scoreText) {
        if (inGameWidget) {
            inGameWidget->SetScoreText(scoreText);
        }
    }

private:
    W_InGame* inGameWidget;
};


