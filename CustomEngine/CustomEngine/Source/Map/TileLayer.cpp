#include "TileLayer.h"
#include "../Graphics/TextureManager.h"
#include <iostream>


TileLayer::TileLayer(int tileSize, int rowCount, int colCount, Tile::Matrix tilematrix, std::vector< std::shared_ptr<TileSet>> tileSets):
	m_TileSize (tileSize), m_ColCount (colCount), m_RowCount (rowCount), m_TileMatrix (tilematrix), m_TileSets (tileSets){

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

void TileLayer::UpdateFloorCollisionInformations(){
	floorCollisionIndexGroups = findFloors(m_TileMatrix);

	heightsBetweenFloors = calculateHeightsBetweenFloors(floorCollisionIndexGroups);
}

std::vector<std::vector<int>> TileLayer::findFloors(const Tile::Matrix& matrix) {
	std::vector<std::vector<int>> floorIndexGroups; // Stores groups of consecutive lines
	std::vector<int> currentGroup; // Current group of consecutive lines

	for (int row = matrix.size()-1; row > 0; row--) {
		bool hasNonZero = false;

		// Checks if the line has at least one non-zero value
		for (int col = matrix[row].size()-1; col > 0 ; col--) {
			if (matrix[row][col] != 0) {
				hasNonZero = true;
				break;
			}
		}

		if (hasNonZero) {
			// Adds current line to group
			currentGroup.push_back(row);
		}
		else if (!currentGroup.empty()) {
			// Ends the current group if the line is null
			floorIndexGroups.push_back(currentGroup);
			currentGroup.clear();
		}
	}

	// If the last group has not been added
	if (!currentGroup.empty()) {
		floorIndexGroups.push_back(currentGroup);
	}

	return floorIndexGroups;
}


std::vector<int> TileLayer::calculateHeightsBetweenFloors(const std::vector<std::vector<int>>& floorCollisionIndexGroups) {
	std::vector<int> heights;
	for (size_t i = 1; i < floorCollisionIndexGroups.size(); ++i) {
		int distanceBetweenIndices = (floorCollisionIndexGroups[i].back() - floorCollisionIndexGroups[i - 1].front());
		if (distanceBetweenIndices > 1) {
			heights.push_back(distanceBetweenIndices * GetTileSize());
		}
	}
	return heights;
}

int TileLayer::GetFloorSize(int floorNumber) {
	int index = 0;
	if (floorNumber > 0) {
		index = floorNumber - 1;
	}
	return floorCollisionIndexGroups[index].size() * GetTileSize();
}

int TileLayer::GetFloorTopPosition(int floorNumber) {
	int index = 0;
	if (floorNumber > 0) {
		index = floorNumber - 1;
	}
	return (floorCollisionIndexGroups[index].back() - 1) * GetTileSize();
}

Transform TileLayer::GetFloorCenterPosition(int floorNumber) {
	int index = 0;
	if (floorNumber > 0) {
		index = floorNumber - 1;
	}
	int middle = (floorCollisionIndexGroups[index].size() - 1) / 2;
	Transform transform;
	transform.Y = (floorCollisionIndexGroups[index][middle]) * GetTileSize();
	transform.X = m_ColCount * GetTileSize()/2;

	return transform;
}

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
