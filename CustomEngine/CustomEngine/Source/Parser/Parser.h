#pragma once

#include "../Map/TileLayer.h"
#include "../Map/TileLayer.h"
#include "../Map/TileMap.h"
#include "../Vendor/tinyXML/tinyxml.h"
#include "../Factory/ObjectFactory.h"
#include <unordered_map>

class Parser{

public:
	static Parser* GetInstance() {
		return s_Instance = (s_Instance != nullptr) ? s_Instance : new Parser();
	}
	void ParseGameObjects(std::string source, std::vector<std::unique_ptr<GameObject>>& targets);
	Properties* GetGameObjectPropertiesById(const std::string& id);
	/*bool ParseTextures(std::string source);*/
	/*TileMap* ParseMap(std::string source);
	TileSet ParseTileSet(TiXmlElement* xmlTileSet);
	TileLayer* ParseTileLayer(TiXmlElement* xmlLayer, TileSetList tileSets, int tileSize, int rowCount, int colCount);*/

private:
	static Parser* s_Instance;
	// Mapa para armazenar as propriedades dos GameObjects pelo id
	std::unordered_map<std::string, Properties*> m_GameObjectProperties;
};
