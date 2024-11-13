#pragma once

#include "Layer.h"
#include <string>
#include <vector>

struct TileSet{
	int FirstId, LastId;
	int RowCount, ColCount;
	int TileCount, TileSize;
	std::string Name, Source;
};

using TileSetList = std::vector<TileSet>;
using TileMatrix = std::vector < std::vector<int> >;

class TileLayer : public Layer{
	public:
		TileLayer(int tileSize, int rowCount, int colCount, TileMatrix tileMap, TileSetList tileSets);
		virtual void Render();
		virtual void Update();
		inline TileMatrix GetTileMap() { return m_TileMap; }
		inline int GetTileSize() { return m_TileSize; }
		inline int GetTileWidth() { return m_ColCount; }
		inline int GetTileHeight() { return m_RowCount; }

	private:
		int m_TileSize;
		int m_RowCount, m_ColCount;

		TileMatrix m_TileMap;
		TileSetList m_TileSets;
};

