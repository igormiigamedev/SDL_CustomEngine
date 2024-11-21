#pragma once

#include <string>
#include "SDL.h"
#include <map>

class TextureManager
{
public:
    static TextureManager* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new TextureManager(); }

    bool Load(std::string id, std::string filename);
    bool ParseTextures(std::string source);
    void Drop(std::string id);
    void Clean();

    void Draw(std::string id, int x, int y, int width, int heigt, float scaleX = 1.0f, float scaleY = 1.0f, float scrollRatio = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void DrawTile(std::string tileSetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void DrawFrame(std::string id, int x, int y, int width, int heigt, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE, float imageScalling = 1.0f);

    SDL_Texture* GetTexture(std::string id) { return m_TextureMap[id]; }
private:
    TextureManager() {}
    std::map<std::string, SDL_Texture*> m_TextureMap;
    static TextureManager* s_Instance;
};
