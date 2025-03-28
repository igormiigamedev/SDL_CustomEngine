#include "TextureManager.h"
#include "../Core/Engine.h"
#include "../Camera/Camera.h"
#include "../Vendor/tinyXML/tinyxml.h"

TextureManager* TextureManager::s_Instance = nullptr;

bool TextureManager::Load(std::string id, std::string filename) {

    SDL_Surface* surface = IMG_Load(filename.c_str());
    
    if (surface == nullptr) {
        SDL_Log("Failed to load texture: %s, %s", filename.c_str(), SDL_GetError());
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);
    if (texture == nullptr) {
        SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
        return false;
    }

    m_TextureMap[id] = texture;
    return true;
}

TextureSize TextureManager::GetTextureSize(std::string id) {
    if (m_TextureMap.find(id) != m_TextureMap.end()) {
        TextureSize size;
        SDL_QueryTexture(m_TextureMap[id], nullptr, nullptr, &size.width, &size.height);
        return size;
    }
    else {
        SDL_Log("Texture ID not found: %s", id.c_str());
        return { 0, 0 }; 
    }
}

bool TextureManager::ParseTextures(std::string source){

    TiXmlDocument xml;
    xml.LoadFile(source);
    if (xml.Error()) {
        std::cout << "Failed to Load: " << source << std::endl;
        return false;
    }

    TiXmlElement* root = xml.RootElement();
    for (TiXmlElement* e=root->FirstChildElement() ; e!= nullptr ; e=e->NextSiblingElement()){
        if (e->Value() == std::string("texture")) {
            std::string id = e->Attribute("id");
            std::string src = e->Attribute("source");
            Load(id, src);
        }

    }
    return true;
}

void TextureManager::Draw(std::string id, int x, int y, float scaleX, float scaleY, float scrollRatio, SDL_RendererFlip flip ) {
    TextureSize textureSize = GetTextureSize(id);

    SDL_Rect srcRect = { 0, 0, textureSize.width, textureSize.height };

    Vector2D cam = Camera::GetInstance()->GetPosition() * scrollRatio;
    SDL_Rect dstRect = { x /*- cam.X*/, y - cam.Y, textureSize.width * scaleX, textureSize.height * scaleY };
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawTile(std::string tileSetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip){
    SDL_Rect srcRect = { frame*tileSize, row*tileSize, tileSize, tileSize }; 

    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect dstRect = { x /*- cam.X*/, y - cam.Y, tileSize, tileSize };  
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[tileSetID], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawFrame(std::string id, int x, int y, int width, int height, int row, int column, SDL_RendererFlip flip, float scaleX, float scaleY) {
    SDL_Rect srcRect = { width * (column -1 ), height * (row - 1), width, height };

    Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect dstRect = { x /*- cam.X*/, y - cam.Y, width * scaleX, height * scaleY };
    SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}


void TextureManager::Drop(std::string id) {
    SDL_DestroyTexture(m_TextureMap[id]);
    m_TextureMap.erase(id);
}

void TextureManager::Clean() {
    std::map<std::string, SDL_Texture*>::iterator it;
    for (it = m_TextureMap.begin(); it != m_TextureMap.end(); it++)
        SDL_DestroyTexture(it->second);

    m_TextureMap.clear();

    SDL_Log("texture map cleaned!");
}

