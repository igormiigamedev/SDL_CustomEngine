#pragma once
#include <SDL.h>
#include <functional>

class Button {
public:
    Button(int x, int y, int w, int h, SDL_Color color, std::function<void()> onClick)
        : rect({ x, y, w, h }), color(color), onClick(onClick) {}

    /*void Update(SDL_Event& event) {
        HandleEvent(event);
    }*/

    void Render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }

    void HandleEvent(SDL_Event& event) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            if (IsMouseOver(mouseX, mouseY)) {
                onClick(); 
            }
        }
    }

private:
    SDL_Rect rect;
    SDL_Color color;
    std::function<void()> onClick;

    // Make sure the mouse is over the button
    bool IsMouseOver(int mouseX, int mouseY) {
        return (mouseX > rect.x && mouseX < rect.x + rect.w &&
            mouseY > rect.y && mouseY < rect.y + rect.h);
    }

    
};
