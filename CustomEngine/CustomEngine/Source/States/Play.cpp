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

bool Play::Init() {
	m_EditMode = false;
	m_Ctxt = Engine::GetInstance()->GetRenderer();


	// --------- Parse Textures

	TextureManager::GetInstance()->ParseTextures("Assets/GameTextures.xml");
	/*Parser::GetInstance()->ParseTextures("Assets/GameTextures.xml");*/

	//---------------------
	if (MapParser::GetInstance()->SetUpXmlMaps()) {
		std::cout << "Failed to SetUp Xml Maps" << std::endl;
	}
	// Add first map
	AddMapAtPosition(0, 0);
	// add map above the first
	std::shared_ptr<TileMap> initialMap;
	initialMap = m_ActiveMaps.front();
	int positionY = -initialMap->GetHeight();
	AddMapAtPosition(1, positionY);
	//---------------------

	m_ParalaxBg.push_back(new ImgLayer("bg", 0, -90, 0.5f, 1.0, 1.0 )); //("bg", 0, -90, 1280, 960, 1.0, 1.0, 0.5f);

	int player_texture_width = 240;
	int player_texture_height = 207;
	float imageScalling = 0.7f;
	Transform playertf, enemytf;
	playertf.X = 50;
	playertf.Y = 2.2*SCREEN_HEIGHT;
	enemytf.X = 50;
	enemytf.Y = 2*SCREEN_HEIGHT - 150;
	/*Properties* playerProps = new Properties("Player_Walk", 50, SCREEN_HEIGHT, player_texture_width, player_texture_height, SDL_FLIP_NONE, imageScalling);
	Properties* enemyProps = new Properties("spikeMan_Walk", 50, SCREEN_HEIGHT - 150, 120, 159, SDL_FLIP_NONE, imageScalling);*/

	/*Properties* playerProps = new Properties(player_texture_width, player_texture_height, imageScalling, imageScalling);*/
	/*Properties* enemyProps = new Properties( 120, 159, imageScalling, imageScalling);*/

	Parser::GetInstance()->ParseGameObjects("Assets/Level1.xml");

	/*std::shared_ptr<GameObject> player = SpawnGameObjectAtLocation<GameObject>(GameObjectType::PLAYER, playertf);*/
	PlayerInstance = SpawnGameObjectAtLocation<Player>(GameObjectType::PLAYER, playertf);
	assert(PlayerInstance != nullptr && "Player object is null!"); // Debugging

	EnemyList.push_back(SpawnGameObjectAtLocation < Enemy >(GameObjectType::ENEMY, enemytf));
	assert(EnemyList.back() != nullptr && "Enemy object is null!"); // Debugging

	if (PlayerInstance == nullptr) {
		std::cout << "Failed to create player!" << std::endl;
		return false;
	}
	if (EnemyList.back() == nullptr) {
		std::cout << "Failed to create enemy!" << std::endl;
		return false;
	}

	Camera::GetInstance()->SetTarget(PlayerInstance->GetOrigin());

	/*Gui::GetInstance()->Init();*/
	std::cout << "play initialized!" << std::endl;
	return true;
}

bool Play::Exit(){
	// Limpa e desaloca todos os mapas ativos
	for (std::shared_ptr<TileMap> map : m_ActiveMaps) {
		map->Clean();
	}
	m_ActiveMaps.clear();

	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ) {
		if (auto gameobj = it->lock()) { // Verifica se o objeto ainda existe
			gameobj->Clean();
			++it;
		}
		else {
			it = m_GameObjects.erase(it); // Remove referências fracas para objetos destruídos
		}
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
		// Atualiza os mapas ativos
		UpdateMaps();

		for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ) {
			if (auto gameobj = it->lock()) { // Verifica se o objeto ainda existe
				gameobj->Update(dt);
				++it;
			}
			else {
				it = m_GameObjects.erase(it); // Remove referências para objetos destruídos
			}
		}

		Camera::GetInstance()->Update(dt);

		if (PlayerInstance && PlayerInstance->IsDead()) {
			std::cout << "Morreu" << std::endl;
			// TODO - Implement and handle Character resets
			/*PlayerInstance.reset();*/ 
		}
	}
}

void Play::Render(){
	SDL_SetRenderDrawColor(m_Ctxt, 48, 96, 130, 255); //SDL_SetRenderDrawColor(m_Ctxt, 124, 218, 254, 255);
	SDL_RenderClear(m_Ctxt);

	//TextureManager::GetInstance()->Draw("bg", 0, -90, 1280, 960, 1.0, 1.0, 0.5f); //BackGround with Parallax

	/*for (auto imgLayer : m_ParalaxBg)
		imgLayer->Render();*/

	for (auto map : m_ActiveMaps) {
		map->Render();
	}

	// Renderiza os GameObjects
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ) {
		if (auto gameobj = it->lock()) { // Verifica se o objeto ainda existe
			gameobj->Draw();
			++it;
		}
		else {
			it = m_GameObjects.erase(it); // Remove referências para objetos destruídos
		}
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
	std::pair<int, TileMatrix> result = JoinMatrixOfActiveMaps();
	int tileSize = result.first;
	TileMatrix matrixOfAllMaps = result.second;
	
	if (tileSize == -1) {
		return; 
	}

	CollisionHandler::GetInstance()->SetCollisionMap(matrixOfAllMaps, tileSize);
}

std::pair<int, TileMatrix> Play::JoinMatrixOfActiveMaps() {
	TileMatrix matrixOfAllMaps;
	int tileSize = -1;  // Invalid value to signal error

	// Estimate the total number of rows to avoid reallocations
	size_t totalRows = EstimateMapTotalRows();

	matrixOfAllMaps.reserve(totalRows);

	// Iterate over active maps
	for (const auto& map : m_ActiveMaps) {
		if (map == nullptr) {
			std::cerr << "Null map encountered in active maps!" << std::endl;
			continue;
		}

		TileLayer* collisionLayer = GetCollisionLayerFromMap(map);
		if (collisionLayer == nullptr) {
			continue;
		}

		// Checks if the tile size is consistent
		if (tileSize == -1) {
			tileSize = collisionLayer->GetTileSize();
		}
		else if (tileSize != collisionLayer->GetTileSize()) {
			std::cerr << "Inconsistent tile sizes between maps!" << std::endl;
			return { -1, TileMatrix() };  // Returns error with empty matrix
		}

		// Adds the tilemap matrix to the total matrix
		const auto& tileMapMatrix = collisionLayer->GetTileMapMatrix();
		if (!tileMapMatrix.empty()) {
			matrixOfAllMaps.insert(matrixOfAllMaps.end(), tileMapMatrix.begin(), tileMapMatrix.end());
		}
	}

	return { tileSize, matrixOfAllMaps };
}

size_t Play::EstimateMapTotalRows() const {
	size_t totalRows = 0;

	for (const auto& map : m_ActiveMaps) {
		if (map && !map->GetMapLayers().empty()) {
			TileLayer* layer = dynamic_cast<TileLayer*>(map->GetMapLayers().back().get());
			if (layer) {
				totalRows += layer->GetTileMapMatrix().size();
			}
		}
	}

	return totalRows;
}

TileLayer* Play::GetCollisionLayerFromMap(const std::shared_ptr<TileMap>& map) const {
	if (map && !map->GetMapLayers().empty()) {
		return dynamic_cast<TileLayer*>(map->GetMapLayers().back().get());
	}
	else {
		std::cerr << "Invalid or empty map layers!" << std::endl;
		return nullptr;
	}
}


void Play::UpdateMaps() {
	// Check the current map closest to the top of the screen
	if (!m_ActiveMaps.empty()) {
		std::shared_ptr<TileMap> topMap = m_ActiveMaps.back();

		// If the top of the map is off screen, load a new one above
		if (Camera::GetInstance()->GetPosition().Y < topMap->GetPosition().Y) {
			int positionY = topMap->GetPosition().Y - topMap->GetHeight();
 			AddMapAtPosition(1, positionY);
		}

		// Check the bottom map for removal
		std::shared_ptr<TileMap> bottomMap = m_ActiveMaps.front();
		if (bottomMap->GetPosition().Y - bottomMap->GetHeight() > Camera::GetInstance()->GetPosition().Y) {
			m_ActiveMaps.erase(m_ActiveMaps.begin());
			UpdateCollisionLayers();
		}
	}
}

void Play::AddMapAtPosition(int type, int YPosition) {
	std::shared_ptr<TileMap> newMap = MapParser::GetInstance()->getRandomTileMapOfType(type);
	newMap->SetPosition(0, YPosition);
	m_ActiveMaps.push_back(newMap);
	UpdateCollisionLayers();
}