#include "CollisionHandler.h"
#include "../Core/Engine.h"
#include <iostream>

CollisionHandler* CollisionHandler::s_Instance = nullptr;

CollisionHandler::CollisionHandler() {
	/*m_CollisionLayer = (TileLayer*)Engine::GetInstance()->GetMap()->GetMapLayers().back();
	m_CollisionLayer = (TileLayer*)Play::GetMap()->GetMapLayers().back();
	m_CollisionTileMap = m_CollisionLayer->GetTileMap();*/
}

void CollisionHandler::SetCollisionMap(TileMatrix tilematrix, int tilesize) {
	m_CollisionTileMatrix = tilematrix;
	m_MapTileSize = tilesize;
	m_MapHeight = tilematrix.size();
	m_MapWidth = tilematrix[0].size();
}

CollisionLocation CollisionHandler::DetectTileCollision(const SDL_Rect& entityBounds) {
    // Calculates the indexes of horizontally wrapped tiles
    int leftTile = ClampToRange(entityBounds.x / m_MapTileSize, 0, m_MapWidth - 1);
    int rightTile = ClampToRange((entityBounds.x + entityBounds.w) / m_MapTileSize, 0, m_MapWidth - 1);

    // Calculates the indexes of vertically wrapped tiles, with circular adjustment
    int topTile = WrapTileIndex(entityBounds.y / m_MapTileSize, m_MapHeight);
    int bottomTile = WrapTileIndex((entityBounds.y + entityBounds.h) / m_MapTileSize, m_MapHeight);

    // Adjusts to avoid vertical disconnections due to circular mapping
    if (topTile > bottomTile) {
        bottomTile = topTile + ( (m_MapHeight - 1) - topTile);
    }

    // Iterates through the tiles in the calculated intervals
    for (int i = leftTile; i <= rightTile; ++i) {
        for (int j = topTile; j <= bottomTile; ++j) {
            if (m_CollisionTileMatrix[j][i] > 0) {
                return (j == bottomTile) ? Below : Top;
            }
        }
    }

    return None; // Nenhuma colisão detectada    
}

int CollisionHandler::ClampToRange(int value, int min, int max) {
    return std::max(min, std::min(value, max));
}

int CollisionHandler::WrapTileIndex(int value, int mapHeight) {
    return (value % mapHeight + mapHeight) % mapHeight;
}


bool CollisionHandler::CheckCollision(SDL_Rect a, SDL_Rect b){
	bool x_overlaps = (a.x < b.x + b.w) && (a.x + a.w > b.x);
	bool y_overlaps = (a.y < b.y + b.h) && (a.y + a.h > b.y);
	return (x_overlaps && y_overlaps);
}
