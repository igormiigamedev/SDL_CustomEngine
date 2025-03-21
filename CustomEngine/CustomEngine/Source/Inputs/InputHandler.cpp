#include "InputHandler.h"
#include "../Core/Engine.h"
#include "../States/GameStateManager.h"

InputHandler* InputHandler::s_Instance = nullptr;

InputHandler::InputHandler() {
	m_KeyStates = SDL_GetKeyboardState(nullptr);
	// Initializes previous states to "not pressed"
	memset(m_PreviousKeyStates, 0, sizeof(m_PreviousKeyStates));
}

void InputHandler::Listen(){
	SDL_Event event;

	// Updates previous states before processing current events
	UpdateKeyStates();

	while (SDL_PollEvent(&event)) {
		switch (event.type){
			case SDL_QUIT:
				Engine::GetInstance()->Quit();
				break;
			case SDL_KEYDOWN:
				KeyDown();
				break;
			case SDL_KEYUP:
				KeyUp();
				break;
			case SDL_MOUSEBUTTONDOWN: {
				const auto& buttons = GameStateManager::GetInstance()->GetCurrentHud()->GetButtonList();

				if (!buttons.empty()) {
					for (auto& button : buttons) {
						button->HandleEvent(event);
					}
				}
				break;
			}
			default:
				break;
		}
	}
}

bool InputHandler::GetKeyDown(SDL_Scancode key){
	return (m_KeyStates[key] == 1);
}

bool InputHandler::GetKeyPressed(SDL_Scancode key) {
	// Checks whether the key was "released" in the previous state and was "pressed" in the current state
	return (m_PreviousKeyStates[key] == 0 && m_KeyStates[key] == 1);
}

void InputHandler::UpdateKeyStates() {
	// Updates the previous state of keys before checking the next state
	memcpy(m_PreviousKeyStates, m_KeyStates, sizeof(m_PreviousKeyStates));
}

int InputHandler::GetAxisKey(Axis axis) {
	switch (axis) {
	case HORIZONTAL:
		if (GetKeyDown(SDL_SCANCODE_D) || GetKeyDown(SDL_SCANCODE_RIGHT)) {
			return 1;
		}
		if (GetKeyDown(SDL_SCANCODE_A) || GetKeyDown(SDL_SCANCODE_LEFT)) {
			return -1;
		}
		break;

	case VERTICAL:
		if (GetKeyDown(SDL_SCANCODE_W) || GetKeyDown(SDL_SCANCODE_UP)) {
			return 1;
		}
		if (GetKeyDown(SDL_SCANCODE_S) || GetKeyDown(SDL_SCANCODE_DOWN)) {
			return -1;
		}
		break;

	default:
		return 0;
	}

}

void InputHandler::KeyUp(){
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void InputHandler::KeyDown(){
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}