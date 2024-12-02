#include "TileLayer.h"
//#include <filesystem>
//#include <SDL.h>
#include "../Graphics/TextureManager.h"
#include <iostream>


TileLayer::TileLayer(int tileSize, int rowCount, int colCount, TileMatrix tileMap, std::vector< std::shared_ptr<TileSet>> tileSets):
	m_TileSize (tileSize), m_ColCount (colCount), m_RowCount (rowCount), m_TileMatrix (tileMap), m_TileSets (tileSets){ 

	for (unsigned int i = 0; i < m_TileSets.size(); i++) {
		TextureManager::GetInstance()->Load(m_TileSets[i]->Name, ResolveRelativePath(m_TileSets[i]->Source) );
	}
}

// Manual override solution
std::string TileLayer::ResolveRelativePath(const std::string& inputPath) {
	std::string outputPath = inputPath;

	// Remove "../" and replace with the desired prefix
	size_t pos = outputPath.find("../");
	while (pos != std::string::npos) {
		// Remove "../"
		outputPath.erase(pos, 3);
		// Replace with base path
		outputPath.insert(pos, "Assets/");
		pos = outputPath.find("../");
	}

	return outputPath;
}

// TODO - More general solution
//std::string TileLayer::ResolvePath(const std::string& inputPath) {
//	// Caminho base (onde sua aplicação está executando)
//	std::string basePath = SDL_GetBasePath();
//
//	// Resolve o caminho relativo com base no diretório de trabalho atual
//	std::filesystem::path resolvedPath = std::filesystem::path(basePath) / inputPath;
//
//	// Normaliza o caminho (remove "../" e outras inconsistências)
//	resolvedPath = std::filesystem::canonical(resolvedPath);
//
//	// Converte para string e retorna
//	return resolvedPath.string();
//}

void TileLayer::Render(){
	for (unsigned int row = 0; row < m_RowCount; row++) {
		for (unsigned int col = 0; col < m_ColCount; col++) {
			int tileID = m_TileMatrix[row][col];

			if (tileID == 0) {
				/*std::cout << "Failed to Render Tile Layer: tileID = 0" << std::endl;*/
				continue;
			}
			else {
				int index = 0;
				if (m_TileSets.size() > 1) {
					for (unsigned int k = 1; k < m_TileSets.size(); k++) {
						if (tileID >= m_TileSets[k]->FirstId && tileID <= m_TileSets[k]->LastId) {
							tileID = tileID + m_TileSets[k]->TileCount - m_TileSets[k]->LastId;
							index = k;
							break;
						}
					}
				}

				TileSet ts = *m_TileSets[index];
				int tileRow = tileID / ts.ColCount;
				int tileCol = tileID - tileRow * ts.ColCount - 1;

				//if this tile is on the last column
				if (tileID % ts.ColCount == 0) {
					tileRow--;
					tileCol = ts.ColCount - 1;
				}
				//int size = 20;
				int x = col * ts.TileSize;
				int y = (m_LayerWorldPosition.Y) + row * ts.TileSize;
				

				TextureManager::GetInstance()->DrawTile(ts.Name, ts.TileSize, x, y, tileRow, tileCol);
			}
		}
	}
}

void TileLayer::Update(){

}
