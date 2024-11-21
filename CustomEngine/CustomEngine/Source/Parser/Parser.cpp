#include "Parser.h"
#include "../Physics/Transform.h"
#include "../Graphics/TextureManager.h"
#include "../Map/MapParser.h"

Parser* Parser::s_Instance = nullptr;

void Parser::ParseGameObjects(std::string source, std::vector<std::unique_ptr<GameObject>>& targets) {

	TiXmlDocument xml;
	xml.LoadFile(source);
	if (xml.Error()) {
		std::cout << "Failed to load: " << source << " " << xml.ErrorDesc() << std::endl;
		return;
	}

	TiXmlElement* root = xml.RootElement();
	for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
		if (e->Value() == std::string("object")) {
			int x, y, width, height, flip = 0;
			double scX, scY = 0;
			const char* objType;
			const char* texID;
			double sratio = 0;
			double rot = 0;
			int category = -1;
			double imgScalling = 1.0;

			objType = e->Attribute("type");
			texID = e->Attribute("texture");

			e->Attribute("x", &x);
			e->Attribute("y", &y);
			e->Attribute("rot", &rot);
			e->Attribute("imgScalling", &imgScalling);

			e->Attribute("w", &width);
			e->Attribute("h", &height);
			e->Attribute("flip", &flip);
			e->Attribute("category", &category);

			SDL_RendererFlip rFlip;
			if (flip == 0) { rFlip = SDL_FLIP_NONE; }
			if (flip == 1) { rFlip = SDL_FLIP_HORIZONTAL; }
			if (flip == 2) { rFlip = SDL_FLIP_VERTICAL; }

			e->Attribute("scX", &scX);
			e->Attribute("scY", &scY);
			e->Attribute("sratio", &sratio);

			Properties* props = new Properties(texID, x, y, width, height, rFlip, imgScalling);
			// Armazena as propriedades no mapa com a chave sendo o tipo (id)
			/*m_GameObjectProperties[objType] = props;*/

			// Você pode adicionar a criação do objeto aqui, se necessário:
			auto object = ObjectFactory::GetInstance()->CreateGameObject(objType, props);

			if (object) {
				targets.push_back(std::move(object));
			}

		}
		std::cout << source << "Parsed!" << std::endl;
	}
}

Properties* Parser::GetGameObjectPropertiesById(const std::string& id) {
	auto it = m_GameObjectProperties.find(id);
	if (it != m_GameObjectProperties.end()) {
		return it->second; // Retorna as propriedades do GameObject com esse id
	}
	std::cout << "GameObject with ID " << id << " not found!" << std::endl;
	return nullptr; // Retorna nullptr se o ID não for encontrado
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