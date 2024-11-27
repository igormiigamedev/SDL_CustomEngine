#include "MapParser.h"

MapParser* MapParser::s_Instance = nullptr;

bool MapParser::Load(){
	int id = m_MapDict.size();
	return Parse(id, "Assets/Maps/map.tmx");
}

bool MapParser::Parse(int id, std::string source){
	TiXmlDocument xml;
	xml.LoadFile(source);

	if (xml.Error()) {
		std::cerr << "failed to load: " << source << std::endl;
		return false;
	}

	TiXmlElement* root = xml.RootElement();
	int rowCount, colCount, tileSize = 0;

	root->Attribute("width", &colCount);
	root->Attribute("height", &rowCount);
	root->Attribute("tilewidth", &tileSize);

	//Parse Tile Sets
	std::vector< std::shared_ptr<TileSet>> tilesets;
	for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
		if (e->Value() == std::string("tileset")) {
			tilesets.push_back(ParseTileSet(e));
		}
	}

	//Parse Layers
	std::shared_ptr<TileMap> gameMap = std::make_shared<TileMap>();
	for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
		if (e->Value() == std::string("layer")) {
			TileLayer* tilelayer = ParseTileLayer(e, tilesets, tileSize, rowCount, colCount);
			gameMap->GetMapLayers().push_back(tilelayer);
			gameMap->SetSize(rowCount* tileSize, colCount* tileSize);
		}
	}

	m_MapDict[id] = gameMap;
	return true;
}

std::shared_ptr <TileSet> MapParser::ParseTileSet(TiXmlElement* xmlTileSet){
	TileSet tileset;
	tileset.Name = xmlTileSet->Attribute("name");
	xmlTileSet->Attribute("firstgid", &tileset.FirstId);

	xmlTileSet->Attribute("tilecount", &tileset.TileCount);
	tileset.LastId = (tileset.FirstId + tileset.TileCount) - 1;

	xmlTileSet->Attribute("columns", &tileset.ColCount);
	tileset.RowCount = tileset.TileCount / tileset.ColCount;
	xmlTileSet->Attribute("tilewidth", &tileset.TileSize);

	/*TiXmlElement* image = xmlTileSet->FirstChildElement();
	tileset.Source = image->Attribute("source");*/

	if (xmlTileSet->FirstChildElement()->Value() == std::string("image"))
		tileset.Source = xmlTileSet->FirstChildElement("image")->Attribute("source");
	else
		tileset.Source = xmlTileSet->Attribute("source");

	return std::make_shared<TileSet>(tileset);

}

TileLayer* MapParser::ParseTileLayer(TiXmlElement* xmlLayer, std::vector< std::shared_ptr<TileSet>> tileSets, int tileSize, int rowCount, int colCount){

	if (xmlLayer == nullptr) {
		throw std::invalid_argument("xmlLayer is NULL.");
	}

	TiXmlElement* data = nullptr;
	for (TiXmlElement* e = xmlLayer->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
		if (e->Value() == std::string("data")) {
			data = e;
			break;
		}
	}

	if (data == nullptr) {
		throw std::runtime_error("Element <data> not found in XML.");
	}

	//parse Layer tile map
	std::string matrix(data->GetText());
	std::istringstream iss(matrix);
	std::string id;

	TileMatrix tilemap(rowCount, std::vector<int>(colCount, 0));

	for (int row = 0; row < rowCount; row++) {
		for (int col = 0; col < colCount; col++) {
			if (!getline(iss, id, ',')) {
				throw std::runtime_error("Insufficient number of tile IDs in the data array.");
			}
			std::stringstream convertor(id);
			convertor >> tilemap[row][col];

			if (!iss.good()) {
				break;
			}
		}
	}

	return (new TileLayer(tileSize, rowCount, colCount, tilemap, tileSets));
}

void MapParser::Clean(){
	std::map<std::string, TileMap>::iterator it;
	/*for (it = m_MapDict.begin(); it != m_MapDict.end(); it++) {
		it->second = NULL;
	}*/
	m_MapDict.clear();
}
