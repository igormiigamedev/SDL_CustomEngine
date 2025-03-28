#pragma once

#include "SDL.H"

enum Axis {HORIZONTAL, VERTICAL};

class InputHandler
{
	public:
		static InputHandler* GetInstance() {
			return s_Instance = (s_Instance != nullptr) ? s_Instance : new InputHandler();
		}

		void Listen();
		bool GetKeyDown(SDL_Scancode key);
		bool GetKeyPressed(SDL_Scancode key);   // Checks if the key was pressed (after being released)

		int GetAxisKey(Axis axis);

	private:
		InputHandler();
		void KeyUp();
		void KeyDown();

		void HandleWidgetButtonEvents(SDL_Event event);

		const Uint8* m_KeyStates;
		static InputHandler* s_Instance;
		Uint8 m_PreviousKeyStates[SDL_NUM_SCANCODES];  // Previous key states

		void UpdateKeyStates();                 // Update previous states
};

