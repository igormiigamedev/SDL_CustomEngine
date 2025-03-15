#include "Play.h"
#include <random>
#include "../Collision/PhysicsWorld.h"
#include "../Effects/ParticleManager.h"
#include "../Graphics/Hud.h"
#include "../States/GameStateManager.h"

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

void Play::SetHud() {
	hud = new HudPlay(m_Ctxt);
	GameStateManager::GetInstance()->SetState(hud);
}

bool Play::Init() {
	m_EditMode = false;
	GameMode::GetInstance()->ResetScore();

	GameState::Init();

	// --------- Parse Textures

	TextureManager::GetInstance()->ParseTextures("Assets/GameTextures_Play.xml");
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
	int positionY = initialMap->GetPosition().Y;
	AddMapAtPosition(1, positionY);
	//---------------------

	m_ParalaxBg.push_back(new ImgLayer("bg", 0, -90, 0.5f, 1.0, 1.0 )); //("bg", 0, -90, 1280, 960, 1.0, 1.0, 0.5f);

	int player_texture_width = 240;
	int player_texture_height = 207;
	Transform playertf;
	playertf.X = 50;

	int height_margin = 50;
	auto collisionLayer = dynamic_cast<TileLayer*>(initialMap->GetMapCollisionLayer().get());

	playertf.Y = collisionLayer->GetFloorTopPosition(1) - player_texture_height - height_margin;

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
	PhysicsWorld::GetInstance()->Reset();

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

		RemoveInactiveObjects();

		for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ) {
			if (auto gameobj = it->lock()) { // Checks if the object still exists
				gameobj->Update(dt);
				++it;
			}
			else {
				it = m_GameObjects.erase(it); // Remove references to destroyed objects
			}
		}

		ParticleManager::GetInstance()->UpdateParticles(dt);

		/*fpsCounter.Update();*/

		HandlePlayerDeath();
	}
}

void Play::HandlePlayerDeath() {
	if (PlayerInstance && PlayerInstance->IsDead()) {
		if (hud->PlayFadeOut(5)) {
			OpenGameOver();
		}
	}
}

void Play::RemoveInactiveObjects() {
	// Remove Collectibles inativos
	CollectibleList.erase(
		std::remove_if(CollectibleList.begin(), CollectibleList.end(),
			[](const std::shared_ptr<Collectible>& collectible) {
				return !collectible->IsActive();
			}),
		CollectibleList.end());
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
	SDL_SetRenderDrawColor(m_Ctxt, 48, 96, 130, 255); 
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

	//Particles
	ParticleManager::GetInstance()->RenderParticles();

	//HUD
	hud->Render(m_Ctxt);

	//Camera
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

void Play::OpenGameOver() {
	GameOver* gameOverState = new GameOver();
	//if (!gameOverState->Init()) {
	//	std::cout << "Failed to initialize GameOver state!" << std::endl;
	//	return;
	//}
	Engine::GetInstance()->ChangeState(gameOverState);
	gameOverState->Init();
}

void Play::PauseGame()
{
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

		// Check the bottom map for removal
		std::shared_ptr<TileMap> bottomMap = m_ActiveMaps.front();
		if (bottomMap->GetPosition().Y - bottomMap->GetHeight() > Camera::GetInstance()->GetPosition().Y) {
			m_ActiveMaps.erase(m_ActiveMaps.begin());
			RemoveOutOfScreenEnemies();
			RemoveOutOfScreenCollectibles();
		}

		// If the top of the map is off screen, load a new one above
		if (Camera::GetInstance()->GetPosition().Y < topMap->GetPosition().Y) {
			int positionY = topMap->GetPosition().Y;
 			AddMapAtPosition(1, positionY);
		}

	}
}

void Play::AddMapAtPosition(int type, int YPosition) {
	// Creates a new map of the specified type and adjusts its position
	std::shared_ptr<TileMap> newMap = MapParser::GetInstance()->getRandomTileMapOfType(type);
	int newMapPositionY = YPosition;
	if (type != 0) {
		newMapPositionY = YPosition - newMap->GetHeight();
	}
 	newMap->SetPosition(0, newMapPositionY);
	m_ActiveMaps.push_back(newMap);

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
	int maxObjectsPerFloor,
	int verticalOffsetMultiplier,
	int minObjectsPerMap,
	int maxObjectsPerMap
) {
	// Validate collision layer
	auto collisionLayer = dynamic_cast<TileLayer*>(map->GetMapCollisionLayer().get());
	if (!collisionLayer) {
		throw std::runtime_error("Invalid collision layer!");
	}

	int totalFloors = collisionLayer->GetAmountOfFloorCollision();
	int maxAvailableFloors = totalFloors - (firstAvailableFloor - 1);

	maxObjectsPerMap = (maxObjectsPerMap < 0) ? maxAvailableFloors : maxObjectsPerMap;
	minObjectsPerMap = (minObjectsPerMap < 0) ? std::ceil((maxAvailableFloors + 1) / 2) : minObjectsPerMap;
	
	int mapTopY = map->GetPosition().Y;
	int verticalOffset = verticalOffsetMultiplier * collisionLayer->GetTileSize();

	// Random number generator setup
	std::random_device randomDevice;
	std::mt19937 randomGenerator(randomDevice());
	std::uniform_int_distribution<> objectCountDistribution(minObjectsPerMap, maxAvailableFloors);
	int numberOfObjectsToSpawn = objectCountDistribution(randomGenerator);
	/*std::cout << "Number to spawn" << numberOfObjectsToSpawn << std::endl;*/

	int amountOfObjectsSpawned = 0;

	// Generation of objects on each floor
	for (int currentFloor = 0; currentFloor < maxAvailableFloors && amountOfObjectsSpawned < numberOfObjectsToSpawn; currentFloor++) {

		int deltaObjectsToSpawn = numberOfObjectsToSpawn - amountOfObjectsSpawned;
		int deltaAvailableFloors = maxAvailableFloors - currentFloor;

		if (deltaObjectsToSpawn < deltaAvailableFloors) {
			// ramdomly decide if will spawn on this floor
			std::uniform_int_distribution<> willSpawnOnThisFloor(0, 1);
			if (willSpawnOnThisFloor(randomGenerator) == 0) {
				/*std::cout << "Pulou o andar" << firstAvailableFloor + i << std::endl;*/
				continue;
			}
		}
		/*std::cout << "Spawnou no andar" << firstAvailableFloor + i  << std::endl;*/

		for (int j = 0; j < maxObjectsPerFloor && amountOfObjectsSpawned < numberOfObjectsToSpawn; j++) {

			if (j > 0) {
				// ramdomly decide if will spawn more objects on this floor
				std::uniform_int_distribution<> willSpawnMoreObjecstOnThisFloor(0, 1);
				if (willSpawnMoreObjecstOnThisFloor(randomGenerator) == 0) {
					/*std::cout << "Pulou o andar" << firstAvailableFloor + i << std::endl;*/
					continue;
				}
			}

			// Random positioning on the X axis
			std::uniform_int_distribution<> xPositionDistribution(1, SCREEN_WIDTH - 90);
			int randomXPosition = xPositionDistribution(randomGenerator);

			// Calculates position at the top of the floor
			int floorTopY = collisionLayer->GetFloorTopPosition(firstAvailableFloor + currentFloor);

			// Defines the final position of the object
			Transform objectPosition;
			objectPosition.X = randomXPosition;
			objectPosition.Y = mapTopY + floorTopY - verticalOffset;

			// Create and add the object to the list
			if constexpr (std::is_same<T, Enemy>::value) {
				EnemyList.push_back(SpawnGameObjectAtLocation<T>(objectType, objectPosition));
				amountOfObjectsSpawned++;
			}
			else if constexpr (std::is_same<T, Collectible>::value) {
				CollectibleList.push_back(SpawnGameObjectAtLocation<T>(objectType, objectPosition));
				amountOfObjectsSpawned++;
			}
		}
	}
}

void Play::SpawnNewEnemyList(int firstAvailableFloor, std::shared_ptr<TileMap>& map) {
	SpawnObjectsOnFloors<Enemy>(GameObjectType::ENEMY, firstAvailableFloor, map, 1, 2);
}

void Play::SpawnNewCollectibleList(int firstAvailableFloor, std::shared_ptr<TileMap>& map) {
	SpawnObjectsOnFloors<Collectible>(GameObjectType::COLLECTIBLE, firstAvailableFloor, map, 1, 4, 2, 4);
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
 	PhysicsWorld::GetInstance()->CleanUpInvalidColliders();
}

void Play::RemoveOutOfScreenEnemies() {
	RemoveOutOfScreenObjects(EnemyList);
}

void Play::RemoveOutOfScreenCollectibles() {
	RemoveOutOfScreenObjects(CollectibleList);
}