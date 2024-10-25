#include "CollisionHandler.h"
#include "../Core/Engine.h"

CollisionHandler* CollisionHandler::s_Instance = nullptr;

CollisionHandler::CollisionHandler() {
	m_CollisionLayer = (TileLayer*)Engine::GetInstance()->GetMap()->GetMapLayers().back();
	m_CollisionTileMap = m_CollisionLayer->GetTileMap();
}

bool CollisionHandler::MapCollision(SDL_Rect a){
	int tileSize = 32;
	int colCount = 40;
	int rowCount = 30;

	int left_tile = a.x / tileSize;
	int right_tile = (a.x + a.w) / tileSize;

	int top_tile = a.y / tileSize;
	int botton_tile = (a.y + a.h) / tileSize;

	//limit range
	if (left_tile < 0) {
		left_tile = 0;
	}
	if (right_tile >= colCount) {
		right_tile = colCount - 1;
	}
	if (top_tile < 0) {
		top_tile = 0;
	}
	if (botton_tile >= rowCount) {
		botton_tile = rowCount - 1;
	}

	for (int i = left_tile; i <= right_tile; ++i) {
		/*for (int j = top_tile; j <= botton_tile; ++j) {*/
			if (m_CollisionTileMap[botton_tile][i] > 0) {
				return true;
			}
		/*}*/
	}
	return false;
}

bool CollisionHandler::CheckCollision(SDL_Rect a, SDL_Rect b){
	bool x_overlaps = (a.x < b.x + b.w) && (a.x + a.w > b.x);
	bool y_overlaps = (a.y < b.y + b.h) && (a.y + a.h > b.y);
	return (x_overlaps && y_overlaps);
 }
