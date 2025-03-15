#pragma once
#include "Hud.h"

class HudGameOver : public Hud {
public:
    HudGameOver(SDL_Renderer* renderer);
    void Render(SDL_Renderer* renderer) override;
    void Init(SDL_Renderer* renderer);
    void LoadTextures() override;

    void RenderTopHighScoresHUD(SDL_Renderer* renderer);
    void RenderGameOverBaseHud(SDL_Renderer* renderer);

    void SetHighScoreScreenIsOpen(bool isOpen) { highScoreScreenIsOpen = isOpen; }
    bool HighScoreScreenIsOpen() const { return highScoreScreenIsOpen; }

private:
    bool highScoreScreenIsOpen = false;
    PanelInfos hudBasePanel;
    PanelInfos scorePanel;
    PanelInfos highScorePanel;

    std::unique_ptr<TextBox> scoreTextBox;
    std::vector<std::unique_ptr<TextBox>> topHighScores;
};

