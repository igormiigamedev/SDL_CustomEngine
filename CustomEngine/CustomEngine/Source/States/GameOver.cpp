#include "GameOver.h"
#include "../Core/Engine.h"
#include "../Graphics/TextureManager.h"
#include "../Text/TextManager.h"
#include "../GameMode/GameMode.h"
#include "../Inputs/InputHandler.h"
#include "Play.h"

const std::string GAME_OVER_BASE_UI = "gameover_base_ui";
const std::string GAME_OVER_Top_HIGHSCORES_UI = "gameover_top_highscores_ui";

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
    LoadUI();
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
    DrawGameOverBaseHud();

    if (highScoreScreenIsOpen) {
        RenderTopHighScoresHUD();
    }

    // Update the screen
    SDL_RenderPresent(m_Ctxt);
}

void GameOver::DrawGameOverBaseHud() {
    //Hud Offset
    const int yOffset = 20;
    const int xOffset = 70;

    //Base Hud Infos
    const int HudWidth = 818;
    const int HudHeight = 728;

    //Score Box Infos
    const int ScoreBoxWidth = 350;
    const int ScoreBoxHeight = 106;
    const int ScoreBoxPosX= 230;
    const int ScoreBoxPosY = 350;

    std::string scoreText = std::to_string(GameMode::GetInstance()->GetScore());


    TextureManager::GetInstance()->Draw(GAME_OVER_BASE_UI, xOffset, yOffset, HudWidth, HudHeight, 1.0f, 1.0f);

    SDL_Rect scoreBoxTransform = { ScoreBoxPosX + xOffset, ScoreBoxPosY + yOffset, ScoreBoxWidth, ScoreBoxHeight };

    SDL_Rect scoreTextTransform = TextManager::GetInstance()->CentralizeTextAtBox(scoreBoxTransform, scoreText);

    TextManager::GetInstance()->RenderText(m_Ctxt, scoreText, scoreTextTransform, { 255, 255, 255, 255 }, 100);
}


void GameOver::LoadUI() {
	TextureManager::GetInstance()->Load(GAME_OVER_BASE_UI, "Assets/UI/BaseUI.png");
    TextureManager::GetInstance()->Load(GAME_OVER_Top_HIGHSCORES_UI, "Assets/UI/TopHighScores.png");
}


void GameOver::RenderTopHighScoresHUD() const {
    //Hud Offset
    const int yOffset = 20;
    const int xOffset = 70;

    //Base Hud Infos
    const int HudWidth = 818;
    const int HudHeight = 728;

    TextureManager::GetInstance()->Draw(GAME_OVER_Top_HIGHSCORES_UI, xOffset, yOffset, HudWidth, HudHeight, 1.0f, 1.0f);

    int textHeight = 60; // height of the rectangle where the text will be drawn
    int textWidth = 120; // width of the rectangle where the text will be drawn
    int ptsize = 0;
    if (textHeight < textWidth) {
        ptsize = textHeight * 1.5;
    }
    else {
        ptsize = textWidth * 1.5;
    }

    // Load font
    /*TTF_Font* font = TTF_OpenFont("Assets/Fonts/flying_bubble/Flying Bubble.otf", ptsize);
    if (!font) {
        std::cerr << "Error loading font: " << TTF_GetError() << std::endl;
        return;
    }*/

    SDL_Color textColor = { 255, 255, 255, 255 };

    // Show top 10 highscores
    int yTextOffset = 100;
    for (size_t i = 0; i < GameMode::GetInstance()->GetTopHighScores().size(); ++i) {
        std::string highScoreText = std::to_string(i + 1) + ". " + std::to_string(GameMode::GetInstance()->GetTopHighScores()[i]);
        SDL_Rect hsRect = { SCREEN_WIDTH / 2 - textWidth / 2, static_cast<int>(yTextOffset + (i * textHeight)), textWidth, textHeight };
        TextManager::GetInstance()->RenderText(m_Ctxt, highScoreText, hsRect, textColor, ptsize);
    }

    /*TTF_CloseFont(font);*/
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