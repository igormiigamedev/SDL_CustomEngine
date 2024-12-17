#include "Play.h"
#include <random>
#include "../Collision/PhysicsWorld.h"
//#include "Gui.h"

Play::Play(){}

void Play::Events(){
	if (!m_EditMode && InputHandler::GetInstance()->GetKeyPressed(SDL_SCANCODE_P)) {
		Play::Exit();
		Play::Init();
	}

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

	Parser::GetInstance()->ParseGameObjects("Assets/GameObjectsInfos.xml");

	//---------------------
	if (!MapParser::GetInstance()->SetUpXmlMaps()) {
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
	playertf.Y = 1.65*SCREEN_HEIGHT;
	enemytf.X = 50;
	enemytf.Y = SCREEN_HEIGHT;

	PlayerInstance = SpawnGameObjectAtLocation<Player>(GameObjectType::PLAYER, playertf);
	assert(PlayerInstance != nullptr && "Player object is null!"); // Debugging

	if (PlayerInstance == nullptr) {
		std::cout << "Failed to create player!" << std::endl;
		return false;
	}

	Camera::GetInstance()->SetTarget(PlayerInstance);

	/*Gui::GetInstance()->Init();*/
	std::cout << "play initialized!" << std::endl;
	return true;
}

bool Play::Exit(){

	for (std::shared_ptr<TileMap> map : m_ActiveMaps) {
		map->Clean();
	}
	m_ActiveMaps.clear();

	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ) {
		if (auto gameobj = it->lock()) { // Checks if the object still exists
			gameobj->Clean();
			++it;
		}
		else {
			it = m_GameObjects.erase(it); // Remove weak references to destroyed objects
		}
	}

	m_GameObjects.clear();
	PlayerInstance.reset(); 
	EnemyList.clear();
	CollectibleList.clear();

	TextureManager::GetInstance()->Clean();
	std::cout << "Exit Play" << std::endl;

	return true;
}

void Play::Update(){

	Events();

	if (!m_EditMode) {
		float dt = Timer::GetInstance()->GetDeltaTime();

		Camera::GetInstance()->Update(dt);

		UpdateMaps();

		PhysicsWorld::GetInstance()->Update();

		for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ) {
			if (auto gameobj = it->lock()) { // Checks if the object still exists
				gameobj->Update(dt);
				++it;
			}
			else {
				it = m_GameObjects.erase(it); // Remove references to destroyed objects
			}
		}

		if (PlayerInstance && PlayerInstance->IsDead()) {
			std::cout << "Morreu" << std::endl;

			/*DestroyPlayer();*/
		}
	}
}

void Play::DestroyPlayer() {
	if (PlayerInstance) {
		PlayerInstance.reset();
	}
}


void Play::DestroyEnemy(std::shared_ptr<Enemy> enemy) {
	if (!enemy) return;

	// Remove from enemies list
	EnemyList.erase(
		std::remove(EnemyList.begin(), EnemyList.end(), enemy),
		EnemyList.end()
	);

	enemy.reset();
}

void Play::DestroyCollectible(std::shared_ptr<Collectible> collectible) {
	if (!collectible) return;

	// Remove from Collectible list
	CollectibleList.erase(
		std::remove(CollectibleList.begin(), CollectibleList.end(), collectible),
		CollectibleList.end()
	);

	collectible.reset();
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

	// Renders the GameObjects
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ) {
		if (auto gameobj = it->lock()) { // Checks if the object still exists
			gameobj->Draw();
			++it;
		}
		else {
			it = m_GameObjects.erase(it); // Remove references to destroyed objects
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
	std::pair<int, Tile::Matrix> result = JoinMatrixOfActiveMaps();
	int tileSize = result.first;
	Tile::Matrix matrixOfCurrentMapsOnScreen = result.second;
	
	if (tileSize == -1) {
		return; 
	}

	CollisionHandler::GetInstance()->SetCollisionMap(matrixOfCurrentMapsOnScreen, tileSize);
}

std::pair<int, Tile::Matrix> Play::JoinMatrixOfActiveMaps() {
	Tile::Matrix matrixOfCurrentMapsOnScreen;
	int tileSize = -1;  // Invalid value to signal error

	// Estimate the total number of rows to avoid reallocations
	size_t totalRows = EstimateMapTotalRows();

	matrixOfCurrentMapsOnScreen.reserve(totalRows);

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
			return { -1, Tile::Matrix() };  // Returns error with empty matrix
		}

		// Adds the tilemap matrix to the total matrix
		const auto& tileMapMatrix = collisionLayer->GetTileMapMatrix();
		if (!tileMapMatrix.empty()) {
			matrixOfCurrentMapsOnScreen.insert(matrixOfCurrentMapsOnScreen.end(), tileMapMatrix.begin(), tileMapMatrix.end());
		}
	}

	return { tileSize, matrixOfCurrentMapsOnScreen };
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
		return dynamic_cast<TileLayer*>(map->GetMapCollisionLayer().get());
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
			RemoveOutOfScreenEnemies();
			RemoveOutOfScreenCollectibles();
		}
	}
}

void Play::AddMapAtPosition(int type, int YPosition) {
	// Creates a new map of the specified type and adjusts its position
	std::shared_ptr<TileMap> newMap = MapParser::GetInstance()->getRandomTileMapOfType(type);
 	newMap->SetPosition(0, YPosition);
	m_ActiveMaps.push_back(newMap);

	// Updates the active map's collision layers
	UpdateCollisionLayers();

	// Sets the first available floor depending on the map type
	int firstAvailableFloor = (type == 0) ? 2 : 1;

	// Generates the enemy and collectible lists for the new map
	SpawnNewEnemyList(firstAvailableFloor, newMap);
	SpawnNewCollectibleList(firstAvailableFloor, newMap);
}

template<typename T>
void Play::SpawnObjectsOnFloors(
	GameObjectType objectType,
	int firstAvailableFloor,
	std::shared_ptr<TileMap>& map,
	int objectsPerFloor,
	int verticalOffsetMultiplier
) {
	// Check the map collision layer
	auto collisionLayer = dynamic_cast<TileLayer*>(map->GetMapCollisionLayer().get());
	if (!collisionLayer) {
		throw std::runtime_error("Invalid collision layer!");
	}

	int totalFloors = collisionLayer->GetAmountOfFloorCollision();
	int maxAvailableFloors = totalFloors - (firstAvailableFloor - 1);
	int minObjectsPerMap = std::ceil((maxAvailableFloors + 1) / 2);

	int mapBottomY = map->GetPosition().Y + map->GetHeight();
	int verticalOffset = verticalOffsetMultiplier * collisionLayer->GetTileSize();

	// Random number generator setup
	std::random_device randomDevice;
	std::mt19937 randomGenerator(randomDevice());
	std::uniform_int_distribution<> objectCountDistribution(minObjectsPerMap, maxAvailableFloors);

	// Determines the number of objects to spawn
	int numberOfObjectsToSpawn = objectCountDistribution(randomGenerator);

	// Generation of objects on each floor
	for (int i = 0; i < numberOfObjectsToSpawn; i++) {
		for (int j = 0; j < objectsPerFloor; j++) {
			// Random positioning on the X axis
			std::uniform_int_distribution<> xPositionDistribution(1, SCREEN_WIDTH);
			int randomXPosition = xPositionDistribution(randomGenerator);

			// Calculates position at the top of the floor
			int floorTopY = collisionLayer->GetFloorTopPosition(firstAvailableFloor + i);

			// Defines the final position of the object
			Transform objectPosition;
			objectPosition.X = randomXPosition;
			objectPosition.Y = mapBottomY - floorTopY - verticalOffset;

			// Create and add the object to the list
			if constexpr (std::is_same<T, Enemy>::value) {
				EnemyList.push_back(SpawnGameObjectAtLocation<T>(objectType, objectPosition));
			}
			else if constexpr (std::is_same<T, Collectible>::value) {
				CollectibleList.push_back(SpawnGameObjectAtLocation<T>(objectType, objectPosition));
			}
		}
	}
}

void Play::SpawnNewEnemyList(int firstAvailableFloor, std::shared_ptr<TileMap>& map) {
	SpawnObjectsOnFloors<Enemy>(GameObjectType::ENEMY, firstAvailableFloor, map, 1);
}

void Play::SpawnNewCollectibleList(int firstAvailableFloor, std::shared_ptr<TileMap>& map) {
	SpawnObjectsOnFloors<Collectible>(GameObjectType::COLLECTIBLE, firstAvailableFloor, map, 1);
}

template<typename T>
void Play::RemoveOutOfScreenObjects(std::vector<std::shared_ptr<T>>& objectList) {
	// Camera position, which defines the visible area
	int cameraTop = Camera::GetInstance()->GetPosition().Y;
	int cameraBottom = cameraTop + SCREEN_HEIGHT;

	// Iterates over the list of objects and removes those that are off-screen
	for (auto it = objectList.begin(); it != objectList.end(); ) {
		auto object = *it;
		int objectY = object->GetTransform().Y;

		// Checks if the object is below the screen
		if (objectY > cameraBottom) {
			it = objectList.erase(it);  // Remove the object
			std::cout << "Objeto removido" << std::endl;
		}
		else {
			++it;
		}
	}
}

void Play::RemoveOutOfScreenEnemies() {
	RemoveOutOfScreenObjects(EnemyList);
}

void Play::RemoveOutOfScreenCollectibles() {
	RemoveOutOfScreenObjects(CollectibleList);
}