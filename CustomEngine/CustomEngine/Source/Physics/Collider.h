#pragma once

#include "SDL.h"
#include "../Camera/Camera.h"
#include <unordered_map>
#include "../Collision/CollisionManager.h"    

class Collider : public CollisionManager{

	public:

		inline SDL_Rect Get() { return m_Box; }
		inline void SetBuffer(int x, int y, int w, int h) { m_Buffer = { x, y, w, h }; }

		void SetProperties( int x, int y, int w, int h) {
			m_Box = {
				x - m_Buffer.x,
				y - m_Buffer.y,
				w - m_Buffer.w,
				h - m_Buffer.h
			};
		}

		void SetPositionX(int x) { m_Box.x = x; }

		void SetPositionY(int y) { m_Box.y = y; }

		void DrawDebugCollider() {
			Vector2D cam = Camera::GetInstance()->GetPosition();
			SDL_Rect debugBox = m_Box;
			/*m_Box.x -= cam.X;*/
			debugBox.y -= cam.Y;
			SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &debugBox); 
		}

	private:
		SDL_Rect m_Box;
		SDL_Rect m_Buffer;
};

