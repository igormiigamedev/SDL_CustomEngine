#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
#include "SDL_image.h"
#include "../Object/GameObject.h"
#include "../Factory/ObjectFactory.h"
#include "../States/GameState.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 750 //750 //980

class Engine {

public:
    static Engine* GetInstance() {
        return s_Instance = (s_Instance != nullptr) ? s_Instance : new Engine();
    }

    bool Init();
    bool Clean();
    void Quit();

    void Update();
    void Render();
    void Events();

    void PopState();
    void PushState(GameState* current);
    void ChangeState(GameState* target);

    inline bool IsRunning() { return m_IsRunning; }
    inline SDL_Renderer* GetRenderer() { return m_Renderer; }
    inline SDL_Window* GetMainWindow() { return m_Window; }

private:
    Engine() {}
    bool m_IsRunning;

    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    static Engine* s_Instance;

    std::vector<GameObject*> m_GameObjects;
    std::vector<GameState*> m_States;

};

#endif // ENGINE_H

