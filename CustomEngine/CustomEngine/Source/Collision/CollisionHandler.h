#pragma once

#include "SDL.h"
#include <vector>
#include "../Map/TileLayer.h"
#include "../Map/TileMap.h"
#include "../Physics/Collider.h"
#include "../Physics/RectCollider.h"
#include "../Physics/CircleCollider.h"

enum CollisionLocation { None, Top, Below };

class CollisionHandler{

	public:
		CollisionLocation DetectTileCollision( Collider& entityBounds);

		bool AreCloseTo(int checker, int other);
		int ClampToRange(int value, int min, int max);
		int WrapTileIndex(int value, int mapHeight);
		bool CheckCollision(Collider& a, Collider& b);
		void SetCollisionMap(Tile::Matrix tilematrix, int tilesize);

		bool CheckRectCircleCollision(const CircleCollider& circleCollider, const RectCollider& rectCollider) const;
		bool CheckCircleCollision(const CircleCollider& checker, const CircleCollider& other) const;

		inline static CollisionHandler* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new CollisionHandler(); }

	private:
		CollisionHandler();
		
		Tile::Matrix m_CollisionTileMatrix;
		TileLayer* m_CollisionLayer;
		static CollisionHandler* s_Instance;

		int m_MapTileSize;
		int m_MapHeight;
		int m_MapWidth;
};

