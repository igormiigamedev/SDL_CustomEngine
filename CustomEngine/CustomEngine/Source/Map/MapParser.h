#pragma once

#include <map>
#include <string>
#include "TileMap.h"
#include "TileLayer.h"
#include "../Vendor/tinyXML/tinyxml.h"

class MapParser{
	public:
		bool SetUpXmlMaps();
		void Clean();

		std::shared_ptr<TileMap> getRandomTileMapOfType(int type);

		static MapParser* GetInstance() {
			return s_Instance = (s_Instance != nullptr) ? s_Instance : new MapParser();
		}

	private:
		bool Parse(int id, std::string source);
		std::shared_ptr <TileSet> ParseTileSet(TiXmlElement* xmlTileSet);
		std::unique_ptr <TileLayer> ParseTileLayer(TiXmlElement* xmlLayer, std::vector< std::shared_ptr<TileSet>> tileSets, int tileSize, int rowCount, int colCount);

	private:
		MapParser() {}
		static MapParser* s_Instance;

		std::map<int, std::vector<std::shared_ptr<TileMap>>> m_templateMapDict;
};

