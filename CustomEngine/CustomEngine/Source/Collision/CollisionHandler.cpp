#include "CollisionHandler.h"
#include "../Core/Engine.h"
#include <iostream>

CollisionHandler* CollisionHandler::s_Instance = nullptr;

CollisionHandler::CollisionHandler() {
	m_CollisionLayer = (TileLayer*)Engine::GetInstance()->GetMap()->GetMapLayers().back();
	m_CollisionTileMap = m_CollisionLayer->GetTileMap();
}

void CollisionHandler::SetCollisionMap(TileMap tilemap, int tilesize) {
	m_CollisionTileMap = tilemap;
	m_MapTileSize = tilesize;
	m_MapHeight = tilemap.size();
	m_MapWidth = tilemap[0].size();
}

CollisionLocation CollisionHandler::MapCollision(SDL_Rect a){ 
	/*int tileSize = 32;
	int colCount = 40;
	int rowCount = 30;*/

	int left_tile = a.x / m_MapTileSize;
	int right_tile = (a.x + a.w) / m_MapTileSize;

	int top_tile = a.y / m_MapTileSize;
	int botton_tile = (a.y + a.h) / m_MapTileSize;

	//limit range
	if (left_tile < 0) {
		left_tile = 0;
	}
	if (right_tile >= m_MapWidth) {
		right_tile = m_MapWidth - 1;
	}
	if (top_tile < 0) {
		top_tile = 0;
	}
	if (botton_tile >= m_MapHeight) {
		botton_tile = m_MapHeight - 1;
	}

	for (int i = left_tile; i <= right_tile; ++i) {
		for (int j = top_tile; j <= botton_tile; ++j) {
			if (m_CollisionTileMap[j][i] > 0) {
				/*std::cout << "Valor de j: " << j << std::endl;
				std::cout << "Valor de botton_tile: " << botton_tile << std::endl;
				std::cout << "Valor de top_tile: " << top_tile << std::endl;*/
				if (j == botton_tile ) {
					return Below;
				}
				else {
					return Top;
				}
			}
		}
	}
	return None;
}

bool CollisionHandler::CheckCollision(SDL_Rect a, SDL_Rect b){
	bool x_overlaps = (a.x < b.x + b.w) && (a.x + a.w > b.x);
	bool y_overlaps = (a.y < b.y + b.h) && (a.y + a.h > b.y);
	return (x_overlaps && y_overlaps);
}
