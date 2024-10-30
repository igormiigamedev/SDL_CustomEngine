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


Engine* Engine::s_Instance = nullptr;

bool Engine::Init() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
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

    if (!MapParser::GetInstance()->Load()) {
        std::cout << "Failed to load map" << std::endl;
    }
    m_LevelMap = MapParser::GetInstance()->GetMap("MAP");

    // load texture
    TextureManager::GetInstance()->ParseTextures("Assets/GameTextures.xml");

    int player_texture_width = 240;
    int player_texture_height = 207;
    float imageScalling = 0.7f;

    Player* player = new Player(new Properties("Player_Walk", 50, SCREEN_HEIGHT, player_texture_width, player_texture_height, SDL_FLIP_NONE, imageScalling));
    Enemy* enemy = new Enemy(new Properties("spikeMan_Walk", 50, SCREEN_HEIGHT - 150, 120, 159, SDL_FLIP_NONE, imageScalling));
    m_GameObjects.push_back(player);
    m_GameObjects.push_back(enemy);

    Camera::GetInstance()->SetTarget(player->GetOrigin());
    return m_IsRunning = true;
}

void Engine::Update() {
    float dt = Timer::GetInstance()->GetDeltaTime();
    m_LevelMap->Update();

    for (unsigned int i = 0; i != m_GameObjects.size(); i++) {
        m_GameObjects[i]->Update(dt);
    }


    Camera::GetInstance()->Update(dt);
}

void Engine::Render() {
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);

    TextureManager::GetInstance()->Draw("bg", 0, -90, 1280, 960, 1.0, 1.0, 0.5f); //BackGround with Parallax
    m_LevelMap->Render();

    for (unsigned int i = 0; i != m_GameObjects.size(); i++) {
        m_GameObjects[i]->Draw();
    }


    SDL_RenderPresent(m_Renderer);
}

void Engine::Events() {
    InputHandler::GetInstance()->Listen();
}

bool Engine::Clean() {
    for (unsigned int i = 0; i != m_GameObjects.size(); i++) {
        m_GameObjects[i]->Clean();
    }

    TextureManager::GetInstance()->Clean();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
    return true;
}

void Engine::Quit() {
    m_IsRunning = false;
}
