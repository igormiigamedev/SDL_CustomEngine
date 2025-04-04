#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

constexpr const char* FONT_TYPE = "Assets/Fonts/montara_bold/montara-bold-gothic.otf";

class TextBox {
public:
    TextBox(SDL_Renderer* renderer, SDL_Rect boxRect, SDL_Color color, int maxFontSize, const std::string& message);
    ~TextBox();

    void Render(SDL_Renderer* renderer);
    void SetText(const std::string& newMessage);
    void SetBox(SDL_Rect newBox);

private:
    struct TextProps
    {
        int fontSize;
        int textWidth;
        int textHeight;
    };

    SDL_Texture* textTexture;
    SDL_Rect boxRect;
    SDL_Color color;
    int maxFontSize;
    std::string message;
    bool needsUpdate;
    TextProps txtProps;

    void UpdateTexture(SDL_Renderer* renderer);
    TextProps AdjustTextSizeToFit();


};


