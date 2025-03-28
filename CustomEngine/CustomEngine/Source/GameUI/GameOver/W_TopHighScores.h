#pragma once
#include "../../Graphics/Widget.h"
#include "../../GameMode/GameMode.h"

class W_TopHighScores : public Widget {
public:
    W_TopHighScores(SDL_Renderer* renderer) : Widget(renderer) {
        Init();
        Hide(); // Initially hidden
    }

    void Init() override;

protected:
    ImagePanel highScorePanel;
};


