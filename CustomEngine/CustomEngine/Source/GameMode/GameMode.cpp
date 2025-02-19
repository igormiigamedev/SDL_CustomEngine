#include "GameMode.h"
#include "../Core/Engine.h"

GameMode* GameMode::instance = nullptr;

GameMode* GameMode::GetInstance() {
    if (!instance) {
        instance = new GameMode();
    }
    return instance;
}

void GameMode::IncreaseScore(int amount) {
    playerScore += amount;
}

void GameMode::SaveHighScore() {
    if (playerScore > highScore) {
        highScore = playerScore;
    }
}

void GameMode::ResetScore() {
    playerScore = 0;
}

void GameMode::RenderHUD(SDL_Renderer* renderer) const {
    int textHeight = 70; // height of the rectangle where the text will be drawn
    int textWidth = 350; // width of the rectangle where the text will be drawn
    int ptsize = 0;
    if (textHeight < textWidth) {
        ptsize = textHeight * 1.5; 
    }
    else {
        ptsize = textWidth * 1.5; 
    }
            
    // Load font
    TTF_Font* font = TTF_OpenFont("Assets/Fonts/flying_bubble/Flying Bubble.otf", ptsize);
    if (!font) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColor = { 255, 255, 255, 255 };

    std::string scoreText = "Score: " + std::to_string(playerScore) + "  High Score: " + std::to_string(highScore);

    // Create text texture
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!textSurface) {
        std::cerr << "Error creating text surface: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface); 

    if (!textTexture) {
        std::cerr << "Error creating text texture: " << SDL_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    // Set position and size of text on screen
    SDL_Rect textRect;
    textRect.w = textWidth; 
    textRect.h = textHeight; 
    textRect.x = SCREEN_WIDTH/2 - textRect.w/2;  
    textRect.y = 20;  
    

    // Render text on screen
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    // Free up memory
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

int GameMode::GetScore() const {
    return playerScore;
}

int GameMode::GetHighScore() const {
    return highScore;
}
