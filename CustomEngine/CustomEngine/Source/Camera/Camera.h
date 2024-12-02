#pragma once

#include "SDL.h"
#include"../Physics/Point.h"
#include "../Physics/Vector2D.h"
#include "../Core/Engine.h" 
#include <SDL.h>

class Camera{
	public:

		void Update(float dt);
		inline int GetSceneWidth() { return m_SceneWidth; }
		inline int GetSceneHeight() { return m_SceneHeight; }

		inline SDL_Rect GetViewBox() { return m_ViewBox; }
		inline Vector2D GetPosition() { return m_Position; }

		inline void MoveX(float x) { m_Position.X = x; }
		inline void MoveY(float y) { m_Position.Y = y; }

		inline void SetTarget(Point* target) { 
			m_Target = target; 
			SetCameraInitalPosition(0, static_cast<int>(std::round(target->Y)) - 500);
		}

		inline void SetCameraInitalPosition(int x, int y) {
			m_ViewBox = { x, y, SCREEN_WIDTH, SCREEN_HEIGHT };
		}

		inline void SetSceneLimit(int w, int h) { m_SceneWidth = w; m_SceneHeight = h; }

		inline static Camera* GetInstance() {
			return s_Instance = ( s_Instance != nullptr) ? s_Instance : new Camera(); 
		}

	private:
		Camera() { }
		
		Point* m_Target;
		Vector2D m_Position;

		SDL_Rect m_ViewBox;
		static Camera* s_Instance;
		int m_SceneWidth, m_SceneHeight;

};

