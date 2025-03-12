#include "GameOver.h"
#include "../Core/Engine.h"
#include "../Graphics/TextureManager.h"
#include "../Text/TextManager.h"
#include "../GameMode/GameMode.h"
#include "../Inputs/InputHandler.h"
#include "Play.h"
#include "../Graphics/Hud.h"

GameOver::GameOver() {}

void GameOver::Events() {
    if (InputHandler::GetInstance()->GetKeyPressed(SDL_SCANCODE_P)) {
        StartGame();
    }
    if (InputHandler::GetInstance()->GetKeyPressed(SDL_SCANCODE_H)) {
        if (highScoreScreenIsOpen) {
            highScoreScreenIsOpen = false;
        }
        else {
            highScoreScreenIsOpen = true;
        }
    }
}

bool GameOver::Init()
{
	m_Ctxt = Engine::GetInstance()->GetRenderer();
	std::cout << "GameOver initialized" << std::endl;
    Hud::GetInstance()->LoadTextures();
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
    Hud::GetInstance()->RenderGameOverBaseHud(m_Ctxt);
    

    if (highScoreScreenIsOpen) {
        Hud::GetInstance()->RenderTopHighScoresHUD(m_Ctxt);
    }

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