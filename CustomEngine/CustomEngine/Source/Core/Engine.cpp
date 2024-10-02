#include "Engine.h"
#include "../Graphics/TextureManager.h"
#include "../Physics/Transform.h"
#include "../Characters/Player.h"


Engine* Engine::s_Instance = nullptr;
Player* player = nullptr;

bool Engine::Init() {

    if (SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    m_Window = SDL_CreateWindow("Custom Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (m_Window == nullptr) {
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_Renderer == nullptr) {
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false;
    }

    // load texture
    TextureManager::GetInstance()->Load("Player", "Assets/PlayerWalk.png");
    int player_texture_width = 240;
    int player_texture_height = 207;

    player = new Player(new Properties("Player", 50, (SCREEN_HEIGHT - (player_texture_height + 100) ), player_texture_width, player_texture_height));

    /*Vector2D v1(1, 1), v2(1, 1), v3;
    v3 = v1 + v2;

    Transform tf(2, 2);

    tf.Log("Transform: ");
    v3.Log("V3: ");*/

    return m_IsRunning = true;
}

void Engine::Update() 
{
    player->Update(0);
}

void Engine::Render() 
{
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);

    player->Draw();
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        Quit();
        break;
    }
}

bool Engine::Clean() {
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
