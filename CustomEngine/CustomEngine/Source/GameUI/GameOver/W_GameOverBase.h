#pragma once
#include "../../Graphics/Widget.h"
#include "../../GameMode/GameMode.h"
#include "../../States/GameOver.h"

class W_GameOverBase : public Widget {
public:
    W_GameOverBase(SDL_Renderer* renderer, GameOver* gameOverInstance) : Widget(renderer), gameOverInstance(gameOverInstance) {
        Init();
    }

    void Init() override;

protected:
    ImagePanel hudBasePanel;
    ImagePanel scorePanel;

private:
    GameOver* gameOverInstance;
};


