#pragma once

#include <map>
#include <string>
#include "GameMap.h"
#include "TileLayer.h"
#include "../Vendor/tinyXML/tinyxml.h"

class MapParser{
	public:
		bool Load();
		void Clean();

		inline GameMap* GetMaps(std::string id) { return m_MapDict[id]; };
		static MapParser* GetInstance() {
			return s_Instance = (s_Instance != nullptr) ? s_Instance : new MapParser();
		}

	private:
		bool Parse(std::string id, std::string source);
		TileSet ParseTileSet(TiXmlElement* xmlTileSet);
		TileLayer* ParseTileLayer(TiXmlElement* xmlLayer, TileSetList tileSets, int tileSize, int rowCount, int colCount);

	private:
		MapParser() {}
		static MapParser* s_Instance;
		std::map<std::string, GameMap*> m_MapDict;
};

