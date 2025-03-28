#pragma once
#include <SDL_render.h>
#include "../Graphics/Hud.h"

class GameStateManager {
public:
    static GameStateManager* GetInstance() {
        static GameStateManager instance;
        return &instance;
    }

    void SetState(std::unique_ptr<Hud> newHud) {
        currentHud = std::move(newHud); 
    }

    void Render(SDL_Renderer* renderer) {
        if (currentHud) currentHud->Render(renderer);
    }

    Hud* GetCurrentHud() { return currentHud.get(); }

private:
    std::unique_ptr<Hud> currentHud = nullptr;
};


