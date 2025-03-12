#pragma once
#include <SDL.h>
#include <iostream>
#include <string>
#include <SDL_rect.h>


class TextManager{

public:
	static TextManager* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new TextManager(); }
	void RenderText(SDL_Renderer* renderer, const std::string& message, SDL_Rect textRectTransform, SDL_Color color, int fontSize);
	SDL_Rect CentralizeTextAtBox(SDL_Rect boxTransform, const std::string& text);

private:
	static TextManager* s_Instance;
};

