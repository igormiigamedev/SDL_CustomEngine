#include "HudPlay.h"
#include "../Text/TextManager.h"
#include "TextureManager.h"
#include "../Core/Engine.h"

HudPlay::HudPlay(SDL_Renderer* renderer) {
	LoadTextures();
	Init(renderer);
}

void HudPlay::Render(SDL_Renderer* renderer) {
	Hud::Render(renderer);
}

void HudPlay::LoadTextures() {
    TextureManager::GetInstance()->ParseTextures("Assets/HudTextures_Play.xml");
}

void HudPlay::Init(SDL_Renderer* renderer){
	inGameWidget = CreateWidget<W_InGame>(renderer);
}