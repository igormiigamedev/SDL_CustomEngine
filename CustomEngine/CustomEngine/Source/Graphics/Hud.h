#pragma once
#include <SDL_timer.h>
#include <SDL_render.h>
#include <iostream>
#include "../Text/TextBox.h"
#include "../GameMode/GameMode.h"
#include "Button.h"

class Hud
{
public:
	virtual ~Hud() = default; 
	virtual void Render(SDL_Renderer* renderer) {
		RenderFadeOut(renderer);
		for (auto& button : buttonList) {
			button->Render(renderer);
		}
	}; 

	void UpdateButtons(SDL_Event& event) {
		for (auto& button : buttonList) {
			button->HandleEvent(event);
		}
	}

	virtual void LoadTextures() = 0; 

	bool PlayFadeOut(int fadeDuration);
	void RenderFadeOut(SDL_Renderer* renderer) const;

	const std::vector<std::unique_ptr<Button>>& GetButtonList() const {
		return buttonList;
	}

private:

	Uint32 fadeInitTime = 0; // Time the fade init
	bool waitingForFadeOut = false;
	int fadeAlpha = 0;  // Fade opacity level (0 = invisible, 255 = completely black)

protected:
	struct PanelInfos
	{
		std::string textureID;
		int posX;
		int posY;
		int width;
		int height;
	};

	std::vector<std::unique_ptr<Button>> buttonList;
};

