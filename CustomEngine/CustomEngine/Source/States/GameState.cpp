#include "GameState.h"
#include "../Core/Engine.h"

bool GameState::Init() {
	m_Ctxt = Engine::GetInstance()->GetRenderer();
	SetHud();
	return true;
}