#include "GameOver.h"
#include "../Core/Engine.h"
#include "../Graphics/TextureManager.h"
#include "../Text/TextManager.h"
#include "../GameMode/GameMode.h"
#include "../Inputs/InputHandler.h"
#include "Play.h"
#include "../Graphics/Hud.h"
#include "../States/GameStateManager.h"

GameOver::GameOver() {}

void GameOver::Events() {
    if (InputHandler::GetInstance()->GetKeyPressed(SDL_SCANCODE_P)) {
        StartGame();
    }
    if (InputHandler::GetInstance()->GetKeyPressed(SDL_SCANCODE_H)) {
        if (hud->HighScoreScreenIsOpen()) {
            hud->SetHighScoreScreenIsOpen(false);
        }
        else {
            hud->SetHighScoreScreenIsOpen(true);
        }
    }
}

void GameOver::SetHud() {
    hud = new HudGameOver(m_Ctxt);
    GameStateManager::GetInstance()->SetState(hud);
}

bool GameOver::Init()
{
    GameState::Init();
	std::cout << "GameOver initialized" << std::endl;

	return true;
}

bool GameOver::Exit()
{
	return false;
}

void GameOver::Update(){
    Events();
}

void GameOver::Render() {
    // Set background color
    SDL_SetRenderDrawColor(m_Ctxt, 16, 45, 70, 255);
    SDL_RenderClear(m_Ctxt);

    // Designing the Game Over UI
    hud->Render(m_Ctxt);

    // Update the screen
    SDL_RenderPresent(m_Ctxt);
}

//Callbacks
void GameOver::StartGame(){
    Play* playState = new Play();
    if (!playState->Init()) {
    	std::cout << "Failed to initialize play state!" << std::endl;
    	return;
    }
    Engine::GetInstance()->ChangeState(playState);
}