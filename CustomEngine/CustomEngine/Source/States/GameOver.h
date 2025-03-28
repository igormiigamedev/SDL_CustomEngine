#pragma once
#include <SDL.h>
#include <iostream>
#include "GameState.h"
#include "../Graphics/HudGameOver.h"
#include "../States/GameStateManager.h"

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
		Hud* baseHud = GameStateManager::GetInstance()->GetCurrentHud();

		if (!baseHud) {
			throw std::runtime_error("Erro: None active HUD!");
		}

		HudGameOver* hudGameOver = dynamic_cast<HudGameOver*>(baseHud);

		if (!hudGameOver) {
			throw std::runtime_error("Erro: Active HUD is not a HudGameOver!");
		}

		return *hudGameOver;
	}


	static void StartGame();
private:
	static void Settings();
	static void Editor();
	static void Quit();
};

