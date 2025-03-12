#pragma once
#include <SDL.h>
#include <iostream>
#include "GameState.h"

class GameOver : public GameState{

public:
	GameOver();
	void Events();

	virtual bool Init();
	virtual bool Exit();
	virtual void Update();
	virtual void Render();

	void DrawGameOverBaseHud();

	void LoadUI();

private:
	void RenderTopHighScoresHUD() const;
	static void StartGame();
	static void Settings();
	static void Editor();
	static void Quit();

	bool highScoreScreenIsOpen = false;
};

