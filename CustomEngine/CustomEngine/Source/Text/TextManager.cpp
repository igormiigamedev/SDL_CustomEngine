#include "TextManager.h"
#include <SDL_ttf.h>

TextManager* TextManager::s_Instance = nullptr;

void TextManager::RenderText(SDL_Renderer* renderer, const std::string& message, SDL_Rect textRectTransform, SDL_Color color, int fontSize) {

    // Load font
    TTF_Font* font = TTF_OpenFont("Assets/Fonts/flying_bubble/Flying Bubble.otf", fontSize);
    if (!font) {
        SDL_Log("Error loading font: %s", TTF_GetError());
        return;
    }

    // Create text texture
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message.c_str(), color);
    if (!textSurface) {
        SDL_Log("Error creating text surface: %s", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    if (!textTexture) {
        SDL_Log("Error creating text texture: %s", SDL_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect textRect = textRectTransform;
    // Render text on screen
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Free up memory
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

SDL_Rect TextManager::CentralizeTextAtBox(SDL_Rect boxTransform, const std::string& text) {
    SDL_Rect textTransform;
    int letterSize = 40;

    textTransform.w = text.size() * letterSize;
    textTransform.h = static_cast<int>(std::floor((2.0f / 3.0f) * boxTransform.h));
    textTransform.y = boxTransform.y + (boxTransform.h - textTransform.h) / 2;
    textTransform.x = boxTransform.x + ((boxTransform.w - textTransform.w) / 2);

    return textTransform;
}
