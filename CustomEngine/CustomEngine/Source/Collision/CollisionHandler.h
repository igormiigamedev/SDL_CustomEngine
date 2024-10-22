#pragma once

#include "SDL.h"
#include <vector>
#include "../Map/TileLayer.h"
#include "../Map/GameMap.h"

class CollisionHandler{

	public:
		bool MapCollision(SDL_Rect a);
		bool CheckCollision(SDL_Rect a, SDL_Rect b);

		inline static CollisionHandler* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new CollisionHandler(); }

	private:
		CollisionHandler();
		TileMap m_CollisionTileMap;
		TileLayer* m_CollisionLayer;
		static CollisionHandler* s_Instance;
};

