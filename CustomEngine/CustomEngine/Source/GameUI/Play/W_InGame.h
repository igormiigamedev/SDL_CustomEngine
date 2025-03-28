#pragma once
#include "../../Graphics/Widget.h"

class W_InGame : public Widget
{
public:
	W_InGame(SDL_Renderer* renderer) : Widget(renderer) {
		Init();
	}

	void Init() override;
	void SetScoreText(std::string newScoreText);

protected:
	ImagePanel scorePanel;
	TextBox* scoreTextBoxPtr;
};

