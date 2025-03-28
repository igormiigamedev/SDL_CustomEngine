#pragma once
#include <SDL.h>
#include <iostream>
#include "GameState.h"
#include "../Graphics/HudGameOver.h"

class GameOver : public GameState{

public:
	GameOver();
	void Events();

	virtual void SetHud() override;
	virtual bool Init();
	virtual bool Exit();
	virtual void Update();
	virtual void Render();

	HudGameOver& GetHud() {
		return *hud;
	}

	static void StartGame();
private:
	static void Settings();
	static void Editor();
	static void Quit();

	bool highScoreScreenIsOpen = false;

	HudGameOver* hud;
};

