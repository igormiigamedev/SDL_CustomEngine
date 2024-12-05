#include "MapParser.h"
#include <random>

MapParser* MapParser::s_Instance = nullptr;

bool MapParser::SetUpXmlMaps() {
	TiXmlDocument xml;
	std::string sourcePath = "Assets/GameMaps.xml";
	xml.LoadFile(sourcePath);

	if (xml.Error()) {
		std::cerr << "failed to load: " << sourcePath << std::endl;
		return false;
	}

	TiXmlElement* root = xml.RootElement();
	for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
		if (e->Value() == std::string("maps")) {
			int type;
			std::string source;
			source = e->Attribute("source");
			e->Attribute("type", &type);
			Parse(type, source);
		}
		else {
			std::cout<< "failed to find 'maps' in: " << sourcePath << std::endl;
		}
	}
	return true;
}

std::shared_ptr<TileMap> MapParser::getRandomTileMapOfType(int type) {
	// Checks if the type exists in the map
	if (m_templateMapDict.find(type) == m_templateMapDict.end() || m_templateMapDict[type].empty()) {
		return nullptr; 
	}

	// Generates a random index
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, m_templateMapDict[type].size() - 1);

	int randomIndex = dis(gen);

	return std::make_shared<TileMap>(*m_templateMapDict[type][randomIndex]);
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
			std::unique_ptr <TileLayer> tilelayer = ParseTileLayer(e, tilesets, tileSize, rowCount, colCount);
			gameMap->GetMapLayers().push_back(std::move(tilelayer));
			gameMap->SetSize(rowCount* tileSize, colCount* tileSize);
		}
	}

	m_templateMapDict[id].push_back(gameMap);
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

std::unique_ptr <TileLayer> MapParser::ParseTileLayer(TiXmlElement* xmlLayer, std::vector< std::shared_ptr<TileSet>> tileSets, int tileSize, int rowCount, int colCount){

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

	Tile::Matrix tilematrix(rowCount, std::vector<int>(colCount, 0));

	for (int row = 0; row < rowCount; row++) {
		for (int col = 0; col < colCount; col++) {
			if (!getline(iss, id, ',')) {
				throw std::runtime_error("Insufficient number of tile IDs in the data array.");
			}
			std::stringstream convertor(id);
			convertor >> tilematrix[row][col];

			if (!iss.good()) {
				break;
			}
		}
	}
	std::unique_ptr <TileLayer> tileLayer = std::make_unique<TileLayer>(tileSize, rowCount, colCount, tilematrix, tileSets);

	return tileLayer;
}

void MapParser::Clean(){
	std::map<std::string, TileMap>::iterator it;
	m_templateMapDict.clear();
}
