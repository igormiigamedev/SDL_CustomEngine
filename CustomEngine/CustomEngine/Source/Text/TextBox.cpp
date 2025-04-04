#include "TextBox.h"

TextBox::TextBox(SDL_Renderer* renderer, SDL_Rect boxRect, SDL_Color color, int maxFontSize, const std::string& message)
    : textTexture(nullptr), boxRect(boxRect), color(color), maxFontSize(maxFontSize), message(message), needsUpdate(true) {
    UpdateTexture(renderer);
}

TextBox::~TextBox() {
    if (textTexture) {
        SDL_DestroyTexture(textTexture);
    }
}

void TextBox::SetText(const std::string& newMessage) {
    if (message != newMessage) { // Only changes if the text is different
        message = newMessage;
        needsUpdate = true;
    }
}

void TextBox::SetBox(SDL_Rect newBox) {
    if (boxRect.x != newBox.x || boxRect.y != newBox.y || boxRect.w != newBox.w || boxRect.h != newBox.h) {
        boxRect = newBox;
        needsUpdate = true;
    }
}

void TextBox::UpdateTexture(SDL_Renderer* renderer) {
    if (!needsUpdate) return;

    if (textTexture) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }

    int fontSize = AdjustTextSizeToFit().fontSize;
    // Load font
    TTF_Font* font = TTF_OpenFont(FONT_TYPE, fontSize);
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

    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    if (!textTexture) {
        SDL_Log("Error creating text texture: %s", SDL_GetError());
        TTF_CloseFont(font);
        return;
    }

    TTF_CloseFont(font);

    needsUpdate = false;
}

void TextBox::Render(SDL_Renderer* renderer) {
    if (needsUpdate) {
        UpdateTexture(renderer);
    }

    if (!textTexture) return;

    SDL_Rect textRect;
    textRect.w = txtProps.textWidth; 
    textRect.h = txtProps.textHeight;
    textRect.x = boxRect.x + (boxRect.w - textRect.w) / 2;
    textRect.y = boxRect.y + (boxRect.h - textRect.h) / 2;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
}

TextBox::TextProps TextBox::AdjustTextSizeToFit() {
    TextProps txtPropsDefault = { 5, 0, 0 };
    txtProps.fontSize = maxFontSize;

    while (txtProps.fontSize > 5) {
        TTF_Font* font = TTF_OpenFont(FONT_TYPE, txtProps.fontSize);
        if (!font) return txtPropsDefault;

        TTF_SizeText(font, message.c_str(), &txtProps.textWidth, &txtProps.textHeight);
        TTF_CloseFont(font);

        if (txtProps.textWidth <= boxRect.w && txtProps.textHeight <= boxRect.h) {
            return txtProps;
        }

        txtProps.fontSize--;
    }

    return txtPropsDefault;
}
