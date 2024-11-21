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

	if (!MapParser::GetInstance()->Load()) {
		std::cout << "Failed to load map" << std::endl;
	}
	m_LevelMap = MapParser::GetInstance()->GetMap("MAP");
	/*m_LevelMap = Parser::GetInstance()->ParseMap("Assets/Maps/map.tmx");*/

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
	}

	m_ParalaxBg.push_back(new ImgLayer("bg", 0, -90, 0.5f, 1.0, 1.0 )); //("bg", 0, -90, 1280, 960, 1.0, 1.0, 0.5f);

	int player_texture_width = 240;
	int player_texture_height = 207;
	float imageScalling = 0.7f;

	Properties* playerProps = new Properties("Player_Walk", 50, SCREEN_HEIGHT, player_texture_width, player_texture_height, SDL_FLIP_NONE, imageScalling);
	Properties* enemyProps = new Properties("spikeMan_Walk", 50, SCREEN_HEIGHT - 150, 120, 159, SDL_FLIP_NONE, imageScalling);

	/*Parser::GetInstance()->ParseGameObjects("Assets/Level1.xml", m_SceneObjects);*/

	GameObject* player = SpawnGameObject<GameObject>("PLAYER", playerProps);
	assert(player != nullptr && "Player object is null!"); // Debugging
	GameObject* enemy = SpawnGameObject < GameObject >("ENEMY", enemyProps);
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

	m_LevelMap->Clean();
	delete m_LevelMap;

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

		for (auto& gameobj : m_GameObjects) {
			gameobj->Update(dt);
		}
		Camera::GetInstance()->Update(dt);
		m_LevelMap->Update();
	}
}

void Play::Render(){
	SDL_SetRenderDrawColor(m_Ctxt, 48, 96, 130, 255); //SDL_SetRenderDrawColor(m_Ctxt, 124, 218, 254, 255);
	SDL_RenderClear(m_Ctxt);

	//TextureManager::GetInstance()->Draw("bg", 0, -90, 1280, 960, 1.0, 1.0, 0.5f); //BackGround with Parallax

	for (auto imgLayer : m_ParalaxBg)
		imgLayer->Render();

	m_LevelMap->Render();

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
