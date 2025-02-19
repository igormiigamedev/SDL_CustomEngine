#include "Engine.h"
#include "../Graphics/TextureManager.h"
#include "../Physics/Transform.h"
#include "../Characters/Player.h"
#include "../Inputs/InputHandler.h"
#include "../Timer/Timer.h"
#include "../Map/MapParser.h"
#include <ios>
#include "../Camera/Camera.h"
#include "../Characters/Enemy.h"
#include "../States/Play.h"


Engine* Engine::s_Instance = nullptr;

bool Engine::Init() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        return 1;
    }

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    m_Window = SDL_CreateWindow("Custom Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
    if (m_Window == nullptr) {
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_Renderer == nullptr) {
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false;
    }

    // Initialize the game state (Play)
    Play* playState = new Play();
    if (!playState->Init()) {
        std::cout << "Failed to initialize Play state!" << std::endl;
        return false;
    }

    ChangeState(playState);

    return m_IsRunning = true;
}

void Engine::Update() {
    if (!m_States.empty()) {
        m_States.back()->Update();
    }
}

void Engine::Render() {
    if (!m_States.empty()) {
        m_States.back()->Render();
    }
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events() {
    /*if (!m_States.empty()) {
        m_States.back()->Events();
    }*/
    InputHandler::GetInstance()->Listen();
}

void Engine::PushState(GameState* current) {
    m_States.push_back(current);
}

void Engine::PopState() {
    if (!m_States.empty()) {
        m_States.back()->Exit();
        delete m_States.back();
        m_States.pop_back();
    }
}

void Engine::ChangeState(GameState* target) {
    // Remove current state
    if (!m_States.empty()) {
        m_States.back()->Exit();
        delete m_States.back();
        m_States.pop_back();
    }

    // Add the new state
    m_States.push_back(target);
}

bool Engine::Clean() {
    for (unsigned int i = 0; i != m_GameObjects.size(); i++) {
        m_GameObjects[i]->Clean();
    }

    TextureManager::GetInstance()->Clean();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return true;
}

void Engine::Quit() {
    m_IsRunning = false;
}
