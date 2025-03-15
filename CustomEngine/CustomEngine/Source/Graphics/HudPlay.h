#pragma once
#include "Hud.h"

class HudPlay : public Hud {
public:
    HudPlay(SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer) override;
    void LoadTextures() override;

    void Init(SDL_Renderer* renderer);
    void SetScoreText(std::string scoreText) { scoreTextBox->SetText(scoreText); };
    void SetHighScoreText(std::string highScoreText) { highScoreTextBox->SetText(highScoreText); };

private:
    void RenderScore(SDL_Renderer* renderer);
    std::unique_ptr<TextBox> scoreTextBox;
    std::unique_ptr<TextBox> highScoreTextBox;
    PanelInfos scorePanel;
};


