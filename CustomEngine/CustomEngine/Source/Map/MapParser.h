#pragma once

#include <map>
#include <string>
#include "TileMap.h"
#include "TileLayer.h"
#include "../Vendor/tinyXML/tinyxml.h"

class MapParser{
	public:
		bool Load();
		void Clean();

		inline std::shared_ptr<TileMap> GetMap(int id) { return m_MapDict[id]; };
		inline std::shared_ptr<TileMap> GetLastMap() { return m_MapDict.rbegin()->second;};
		static MapParser* GetInstance() {
			return s_Instance = (s_Instance != nullptr) ? s_Instance : new MapParser();
		}

	private:
		bool Parse(int id, std::string source);
		std::shared_ptr <TileSet> ParseTileSet(TiXmlElement* xmlTileSet);
		TileLayer* ParseTileLayer(TiXmlElement* xmlLayer, std::vector< std::shared_ptr<TileSet>> tileSets, int tileSize, int rowCount, int colCount);

	private:
		MapParser() {}
		static MapParser* s_Instance;
		std::map<int, std::shared_ptr<TileMap>> m_MapDict;
};

