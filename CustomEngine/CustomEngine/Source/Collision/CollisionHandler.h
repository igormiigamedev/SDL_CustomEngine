#pragma once

#include "SDL.h"
#include <vector>
#include "../Map/TileLayer.h"
#include "../Map/TileMap.h"

enum CollisionLocation { None, Top, Below };

class CollisionHandler{

	public:
		CollisionLocation DetectTileCollision(const SDL_Rect& entityBounds);
		int ClampToRange(int value, int min, int max);
		int WrapTileIndex(int value, int mapHeight);
		bool CheckCollision(SDL_Rect a, SDL_Rect b);
		void SetCollisionMap(TileMatrix tilematrix, int tilesize);

		inline static CollisionHandler* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new CollisionHandler(); }

	private:
		CollisionHandler();
		
		TileMatrix m_CollisionTileMatrix;
		TileLayer* m_CollisionLayer;
		static CollisionHandler* s_Instance;

		int m_MapTileSize;
		int m_MapHeight;
		int m_MapWidth;
};

