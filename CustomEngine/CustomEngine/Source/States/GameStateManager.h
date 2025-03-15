#pragma once
#include <SDL_render.h>
#include "../Graphics/Hud.h"

class GameStateManager {
public:
    static GameStateManager* GetInstance() {
        static GameStateManager instance;
        return &instance;
    }

    void SetState(Hud* newHud) {
        if (currentHud) delete currentHud;  //  Frees up memory from previous HUD
        currentHud = newHud;
    }

    void Render(SDL_Renderer* renderer) {
        if (currentHud) currentHud->Render(renderer);
    }

    Hud* GetCurrentHud() { return currentHud; }

private:
    Hud* currentHud = nullptr;
};


