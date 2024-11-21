#include "Parser.h"
#include "../Physics/Transform.h"
#include "../Graphics/TextureManager.h"
#include "../Map/MapParser.h"

Parser* Parser::s_Instance = nullptr;

void Parser::ParseGameObjects(std::string source/*, std::vector<std::unique_ptr<GameObject>>& targets*/) {

	TiXmlDocument xml;
	xml.LoadFile(source);
	if (xml.Error()) {
		std::cout << "Failed to load: " << source << " " << xml.ErrorDesc() << std::endl;
		return;
	}

	TiXmlElement* root = xml.RootElement();
	for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
		if (e->Value() == std::string("object")) {
			int width, height;
			double scaleX, scaleY = 0;
			const char* objType;
			/*double sratio = 0;*/
			/*double rot = 0;*/
			/*int category = -1;*/

			e->Attribute("width", &width);
			e->Attribute("height", &height);
			e->Attribute("scaleX", &scaleX);
			e->Attribute("scaleY", &scaleY);
			/*e->Attribute("sratio", &sratio);*/

			Properties* props = new Properties(width, height, scaleX, scaleY);

			objType = e->Attribute("type");
			GameObjectType type = StringToGameObjectType(objType);
			
			try {
				const char* objType = e->Attribute("type");
				GameObjectType type = StringToGameObjectType(objType);

				m_GameObjectProperties[type] = props;
			}
			catch (const std::invalid_argument& ex) {
				std::cout << "Error: " << ex.what() << std::endl;
				// Perform error handling, such as returning or stopping execution
			}

			// You can add object creation here if needed::
			/*auto object = ObjectFactory::GetInstance()->CreateGameObject(objType, props);

			if (object) {
				targets.push_back(std::move(object));
			}*/

		}
	}
	std::cout << source << "Parsed!" << std::endl;
}

Properties* Parser::GetGameObjectPropertiesByType(const GameObjectType type) {
	auto it = m_GameObjectProperties.find(type);
	if (it != m_GameObjectProperties.end()) {
		return it->second; 
	}
	std::cout << "GameObject with ID " << GameObjectTypeToString(type) << " not found!" << std::endl;
	return nullptr;
}

//bool Parser::ParseTextures(std::string source) {
//	return TextureManager::GetInstance()->ParseTextures(source);
//}

//TileMap* Parser::ParseMap(std::string source) {
//}
//
//TileSet Parser::ParseTileSet(TiXmlElement* xmlTileSet) {
//
//}
//
//TileLayer* Parser::ParseTileLayer(TiXmlElement* xmlLayer, TileSetList tileSets, int tileSize, int rowCount, int colCount) {
//
//}