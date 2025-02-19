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

//TODO - Delete using
namespace Tile {
	using Matrix = std::vector<std::vector<int>>;  // Público se incluído no cabeçalho.
}

class TileLayer : public Layer{
	public:
		TileLayer(int tileSize, int rowCount, int colCount, Tile::Matrix tileMatrix, std::vector< std::shared_ptr<TileSet>> tileSets);
		std::string ResolveRelativePath(const std::string& inputPath);
		std::string ResolvePath(const std::string& inputPath);

		void UpdateFloorCollisionInformations();
		std::vector<std::vector<int>> findFloors(const Tile::Matrix& matrix);
		std::vector<int> calculateHeightsBetweenFloors(const std::vector<std::vector<int>>& floorCollisionIndexGroups);

		int GetFloorSize(int floorNumber);

		int GetFloorTopPosition(int floorNumber);

		Transform GetFloorCenterPosition(int floorNumber);

		virtual void Render();
		virtual void Update();

		inline Tile::Matrix& GetTileMapMatrix() { return m_TileMatrix; }
		inline int GetTileSize() { return m_TileSize; }
		inline int GetTileWidth() { return m_ColCount; }
		inline int GetTileHeight() { return m_RowCount; }

		inline int GetAmountOfFloorCollision() { return floorCollisionIndexGroups.size(); }
		const std::vector<std::vector<int>>& GetFloorCollisionIndexGroups() const { return floorCollisionIndexGroups; }
		const std::vector<int>& GetHeightsBetweenFloors() const { return heightsBetweenFloors; }

		std::unique_ptr<Layer> Clone() const override {
			return std::make_unique<TileLayer>(*this);
		}

	private:
		int m_TileSize;
		int m_RowCount, m_ColCount;

		Tile::Matrix m_TileMatrix;
		std::vector< std::shared_ptr<TileSet>> m_TileSets;

		std::vector<std::vector<int>> floorCollisionIndexGroups;
		std::vector<int> heightsBetweenFloors; 
};

