#include "Play.h"
//#include "Gui.h"

Play::Play(){}

void Play::Events(){
	/*if (!m_EditMode && InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_M)) {
		OpenMenu();
	}*/
	/*if (!m_EditMode && InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_ESCAPE)) {
		m_EditMode = true;
	}

	if (m_EditMode && InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_F5)) {
		m_EditMode = false;
	}

	SDL_Delay(100);*/
}

bool Play::Init(){
	m_EditMode = false;
	m_Ctxt = Engine::GetInstance()->GetRenderer();

	
	// --------- Parse Textures
	
	TextureManager::GetInstance()->ParseTextures("Assets/GameTextures.xml");
	/*Parser::GetInstance()->ParseTextures("Assets/GameTextures.xml");*/

	/*if (!MapParser::GetInstance()->Load()) {
		std::cout << "Failed to load map" << std::endl;
	}
	m_LevelMap = MapParser::GetInstance()->GetMap("MAP");

	TileLayer* collisionLayer;
	if (!m_LevelMap->GetMapLayers().empty()) {
		collisionLayer = (TileLayer*)m_LevelMap->GetMapLayers().back();
		int tileSize = collisionLayer->GetTileSize();
		int width = collisionLayer->GetTileWidth() * tileSize;
		int height = collisionLayer->GetTileHeight() * tileSize;

		Camera::GetInstance()->SetSceneLimit(width, height);
		CollisionHandler::GetInstance()->SetCollisionMap(collisionLayer->GetTileMap(), tileSize);
	}
	else {
		std::cout << "Map layers are empty!" << std::endl;
	}*/

	//---------------------
	if (!MapParser::GetInstance()->Load()) {
		std::cout << "Failed to load map" << std::endl;
		return false;
	}

	std::shared_ptr<TileMap> initialMap = MapParser::GetInstance()->GetLastMap();
	m_ActiveMaps.push_back(initialMap);

	if (!MapParser::GetInstance()->Load()) {
		std::cout << "Failed to load map" << std::endl;
		return false;
	}
	// Carregar mapa acima do primeiro
	std::shared_ptr<TileMap> nextMap = MapParser::GetInstance()->GetLastMap();
	nextMap->SetPosition(0, -initialMap->GetHeight());
	m_ActiveMaps.push_back(nextMap);
	//---------------------

	m_ParalaxBg.push_back(new ImgLayer("bg", 0, -90, 0.5f, 1.0, 1.0 )); //("bg", 0, -90, 1280, 960, 1.0, 1.0, 0.5f);

	int player_texture_width = 240;
	int player_texture_height = 207;
	float imageScalling = 0.7f;
	Transform playertf, enemytf;
	playertf.X = 50;
	playertf.Y = SCREEN_HEIGHT;
	enemytf.X = 50;
	enemytf.Y = SCREEN_HEIGHT - 150;
	/*Properties* playerProps = new Properties("Player_Walk", 50, SCREEN_HEIGHT, player_texture_width, player_texture_height, SDL_FLIP_NONE, imageScalling);
	Properties* enemyProps = new Properties("spikeMan_Walk", 50, SCREEN_HEIGHT - 150, 120, 159, SDL_FLIP_NONE, imageScalling);*/

	/*Properties* playerProps = new Properties(player_texture_width, player_texture_height, imageScalling, imageScalling);*/
	/*Properties* enemyProps = new Properties( 120, 159, imageScalling, imageScalling);*/

	Parser::GetInstance()->ParseGameObjects("Assets/Level1.xml");

	GameObject* player = SpawnGameObjectAtLocation<GameObject>(GameObjectType::PLAYER, playertf);
	assert(player != nullptr && "Player object is null!"); // Debugging
	GameObject* enemy = SpawnGameObjectAtLocation < GameObject >(GameObjectType::ENEMY, enemytf);
	assert(enemy != nullptr && "Enemy object is null!"); // Debugging

	if (player == nullptr) {
		std::cout << "Failed to create player!" << std::endl;
		return false;
	}
	if (enemy == nullptr) {
		std::cout << "Failed to create enemy!" << std::endl;
		return false;
	}

	Camera::GetInstance()->SetTarget(player->GetOrigin());

	/*Gui::GetInstance()->Init();*/
	std::cout << "play initialized!" << std::endl;
	return true;
}

bool Play::Exit(){

	/*m_LevelMap->Clean();
	delete m_LevelMap;*/

	// Limpa e desaloca todos os mapas ativos
	for (std::shared_ptr<TileMap> map : m_ActiveMaps) {
		/*if (map) {*/
		map->Clean();
			/*delete map;*/
		//}
	}
	m_ActiveMaps.clear();

	for (auto& gameobj : m_GameObjects) {
		gameobj->Clean();
		/*delete gameobj;*/
	}

	m_GameObjects.clear();
	TextureManager::GetInstance()->Clean();
	std::cout << "Exit Play" << std::endl;

	return true;
}

void Play::Update(){

	Events();

	if (!m_EditMode) {
		float dt = Timer::GetInstance()->GetDeltaTime();

		/*m_LevelMap->Update();*/

		// Atualiza os mapas ativos
		UpdateMaps();

		// Atualiza as camadas de colisão para os mapas ativos
		UpdateCollisionLayers();

		for (auto& gameobj : m_GameObjects) {
			gameobj->Update(dt);
		}
		Camera::GetInstance()->Update(dt);
	}
}

void Play::Render(){
	SDL_SetRenderDrawColor(m_Ctxt, 48, 96, 130, 255); //SDL_SetRenderDrawColor(m_Ctxt, 124, 218, 254, 255);
	SDL_RenderClear(m_Ctxt);

	//TextureManager::GetInstance()->Draw("bg", 0, -90, 1280, 960, 1.0, 1.0, 0.5f); //BackGround with Parallax

	for (auto imgLayer : m_ParalaxBg)
		imgLayer->Render();

	/*m_LevelMap->Render();*/
	for (auto map : m_ActiveMaps) {
		map->Render();
	}

	for (auto& gameobj : m_GameObjects) {
		gameobj->Draw();
	}

	SDL_Rect camera = Camera::GetInstance()->GetViewBox();

	if (m_EditMode) {
		/*Gui::GetInstance()->draw(m_Ctxt);*/
	}

	SDL_RenderCopy(m_Ctxt, nullptr, &camera, nullptr);
	SDL_RenderPresent(m_Ctxt);
}

//CallBack Actions
void Play::OpenMenu(){
	Engine::GetInstance()->ChangeState(new Menu());
}

void Play::PauseGame()
{
}

void Play::UpdateCollisionLayers() {
	// Atualiza as camadas de colisão para os mapas ativos
	TileMatrix matrixOfAllMaps;
	int tileSize = 0;

	// Estimar o número total de linhas para evitar realocações
	size_t totalRows = 0;
	for (const auto& map : m_ActiveMaps) {
		if (map && !map->GetMapLayers().empty()) {
			TileLayer* layer = dynamic_cast<TileLayer*>(map->GetMapLayers().back());
			if (layer) {
				totalRows += layer->GetTileMapMatrix().size();
			}
		}
	}
	matrixOfAllMaps.reserve(totalRows);

	// Iterar sobre mapas ativos
	for (const auto& map : m_ActiveMaps) {
		if (!map) {
			std::cerr << "Null map encountered in active maps!" << std::endl;
			continue;
		}

		if (!map->GetMapLayers().empty()) {
			TileLayer* collisionLayer = dynamic_cast<TileLayer*>(map->GetMapLayers().back());
			if (!collisionLayer) {
				std::cerr << "Invalid layer type for collision!" << std::endl;
				continue;
			}

			// Obtem o tamanho dos tiles e configura a camada de colisão
			if (tileSize == 0) {
				tileSize = collisionLayer->GetTileSize();
			}
			else if (tileSize != collisionLayer->GetTileSize()) {
				std::cerr << "Inconsistent tile sizes between maps!" << std::endl;
				return;
			}

			const auto& tileMapMatrix = collisionLayer->GetTileMapMatrix();

			if (!tileMapMatrix.empty()) {
				matrixOfAllMaps.insert(matrixOfAllMaps.end(), tileMapMatrix.begin(), tileMapMatrix.end());
			}
		}
		else {
			std::cerr << "Map layers are empty for a map!" << std::endl;
		}
	}

	// Configurar o mapa de colisões
	CollisionHandler::GetInstance()->SetCollisionMap(matrixOfAllMaps, tileSize);
}

void Play::UpdateMaps() {
	// Verifica o mapa atual mais próximo ao topo da tela
	if (!m_ActiveMaps.empty()) {
		std::shared_ptr<TileMap> topMap = m_ActiveMaps.back();

		// Se o topo do mapa estiver fora da tela, carregue um novo acima
		if (Camera::GetInstance()->GetPosition().Y < topMap->GetPosition().Y) {
			if (!MapParser::GetInstance()->Load()) {
				std::cout << "Failed to load map" << std::endl;
				return;
			}
			std::shared_ptr<TileMap> newMap = MapParser::GetInstance()->GetLastMap();
			newMap->SetPosition(0, topMap->GetPosition().Y - topMap->GetHeight());
			m_ActiveMaps.push_back(newMap);
		}

		// Verifica o mapa inferior para remoção
		std::shared_ptr<TileMap> bottomMap = m_ActiveMaps.front();
		if (bottomMap->GetPosition().Y - bottomMap->GetHeight() > Camera::GetInstance()->GetPosition().Y) {
			m_ActiveMaps.erase(m_ActiveMaps.begin());
		}
	}
}