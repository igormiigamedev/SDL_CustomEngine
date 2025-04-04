#pragma once
#include <SDL.h>
#include <functional>
#include "../Graphics/TextureManager.h"

class Button {
public:
    Button(int x, int y, int w, int h, std::string textureID, std::function<void()> onClick, SDL_Color color = {0,0,0,0})
        : rect({ x, y, w, h }), color(color), onClick(onClick), textureID(textureID) {}

    /*void Update(SDL_Event& event) {
        HandleEvent(event);
    }*/

    void Render(SDL_Renderer* renderer) {
        /*SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);*/
        /*SDL_RenderFillRect(renderer, &rect);*/
        TextureManager::GetInstance()->Draw(textureID, rect.x, rect.y, 1.0f, 1.0f);
    }

    bool HandleEvent(SDL_Event& event) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (IsMouseOver(mouseX, mouseY)) {
                onClick(); 
                return true;
            }
        }
        return false;
    }

private:
    std::string textureID;
    SDL_Rect rect;
    SDL_Color color;
    std::function<void()> onClick;

    // Make sure the mouse is over the button
    bool IsMouseOver(int mouseX, int mouseY) {
        return (mouseX > rect.x && mouseX < rect.x + rect.w &&
            mouseY > rect.y && mouseY < rect.y + rect.h);
    }

    
};
