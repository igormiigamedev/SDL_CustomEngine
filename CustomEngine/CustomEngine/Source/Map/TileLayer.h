#pragma once

#include "Layer.h"
#include <string>
#include <vector>
#include "TileMap.h"

struct TileSet{
	int FirstId, LastId;
	int RowCount, ColCount;
	int TileCount, TileSize;
	std::string Name, Source;
};

//TODO - Delete using
using TileMatrix = std::vector < std::vector<int> >;

class TileLayer : public Layer{
	public:
		TileLayer(int tileSize, int rowCount, int colCount, TileMatrix tileMap, std::vector< std::shared_ptr<TileSet>> tileSets);
		virtual void Render();
		virtual void Update();
		inline TileMatrix& GetTileMapMatrix() { return m_TileMatrix; }
		inline int GetTileSize() { return m_TileSize; }
		inline int GetTileWidth() { return m_ColCount; }
		inline int GetTileHeight() { return m_RowCount; }

		std::unique_ptr<Layer> Clone() const override {
			return std::make_unique<TileLayer>(*this);
		}

	private:
		int m_TileSize;
		int m_RowCount, m_ColCount;

		TileMatrix m_TileMatrix;
		std::vector< std::shared_ptr<TileSet>> m_TileSets;
};

