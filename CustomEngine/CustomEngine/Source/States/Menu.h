#pragma once
#include <SDL.h>
#include <iostream>
#include "GameState.h"

class Menu : public GameState{

	public:
		Menu();

		virtual void SetHud() override;
		virtual bool Init();
		virtual bool Exit();
		virtual void Update();
		virtual void Render();

	private:
		static void StartGame();
		static void Settings();
		static void Editor();
		static void Quit();
};

