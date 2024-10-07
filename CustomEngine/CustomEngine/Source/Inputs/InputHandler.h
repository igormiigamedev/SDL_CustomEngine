#pragma once

#include "SDL.H"

class InputHandler
{
	public:
		static InputHandler* GetInstance() {
			return s_Instance = (s_Instance != nullptr) ? s_Instance : new InputHandler();
		}

		void Listen();
		bool GetKeyDown(SDL_Scancode key);

	private:
		InputHandler();
		void KeyUp();
		void KeyDown();

		const Uint8* m_KeyStates;
		static InputHandler* s_Instance;
};

