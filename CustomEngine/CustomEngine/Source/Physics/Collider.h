#pragma once

#include "SDL.h"
#include "../Camera/Camera.h"

class Collider{

	public:
		inline SDL_Rect Get() { return m_Box; }
		inline void SetBuffer(int x, int y, int w, int h) { m_Buffer = { x, y, w, h }; }

		void Set( int x, int y, int w, int h) {
			m_Box = {
				x - m_Buffer.x,
				y - m_Buffer.y,
				w - m_Buffer.w,
				h - m_Buffer.h
			};
		}

		void DrawDebugCollider() {
			Vector2D cam = Camera::GetInstance()->GetPosition();
			/*m_Box.x -= cam.X;*/
			m_Box.y -= cam.Y;
			SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &m_Box);
		}

	private:
		SDL_Rect m_Box;
		SDL_Rect m_Buffer;
};

