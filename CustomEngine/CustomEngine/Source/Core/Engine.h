#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
#include "SDL_image.h"
#include "../Map/GameMap.h"
#include "../Object/GameObject.h"
#include "../Factory/ObjectFactory.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 750 //640

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

    inline GameMap* GetMap() { return m_LevelMap; }
    inline bool IsRunning() { return m_IsRunning; }
    inline SDL_Renderer* GetRenderer() { return m_Renderer; }

    template< typename T, typename = std::enable_if_t< std::is_base_of_v< GameObject, T > > >
    T* SpawnGameObject(std::string type, Properties* props) {
        /*T* game_object = new T(props);*/
        T* game_object = ObjectFactory::GetInstance()->CreateObject(type, props);
        m_GameObjects.push_back(game_object);
        return game_object;
    }

private:
    Engine() {}
    bool m_IsRunning;

    GameMap* m_LevelMap;
    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    static Engine* s_Instance;

    std::vector<GameObject*> m_GameObjects;
};

#endif // ENGINE_H

