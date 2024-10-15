#include "TileLayer.h"
#include "../Graphics/TextureManager.h"


TileLayer::TileLayer(int tileSize, int rowCount, int colCount, TileMap tileMap, TileSetList tileSets):
	m_TileSize (tileSize), m_ColCount (colCount), m_RowCount (rowCount), m_TileMap (tileMap), m_TileSets (tileSets){ 

	for (unsigned int i = 0; i < m_TileSets.size(); i++) {
		TextureManager::GetInstance()->Load(m_TileSets[i].Name, "../Assets/Maps" + m_TileSets[i].Source);
	}
}

void TileLayer::Render(){
	for (unsigned int i = 0; i < m_RowCount; i++) {
		for (unsigned int j = 0; j < m_ColCount; j++) {
			int tileID = m_TileMap[i][j];

			if (tileID == 0) {
				continue;
			}
			else {
				int index;
				if (m_TileSets.size() > 1) {
					for (unsigned int k = 1; k < m_TileSets.size(); k++) {
						if (tileID > m_TileSets[k].FirstId && tileID < m_TileSets[k].LastId) {
							tileID = tileID + m_TileSets[k].TileCount - m_TileSets[k].LastId;
							index = k;
							break;
						}
					}
				}

				TileSet ts = m_TileSets[index];
				int tileRow = tileID / ts.ColCount;
				int tileCol = tileID - tileRow * ts.ColCount - 1;

				//if this tile is on the last column
				if (tileID % ts.ColCount == 0) {
					tileRow--;
					tileCol = ts.ColCount - 1;
				}

				TextureManager::GetInstance()->DrawTile(ts.Name, ts.TileSize, j * ts.TileSize, i * ts.TileSize, tileRow, tileCol);
			}
		}
	}
}

void TileLayer::Update()
{
}
