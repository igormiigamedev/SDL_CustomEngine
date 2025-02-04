#pragma once
#include <iostream>
#include "../Object/GameObject.h"
#include "GameState.h"
#include "../Characters/Enemy.h"
#include "../Characters/Player.h"
#include "../Collectibles/Collectible.h"
#include "../Timer/Timer.h"
#include "Menu.h"
#include "../Inputs/InputHandler.h"
#include "../Camera/Camera.h"
#include "../Map/TileMap.h"
#include "../Map/MapParser.h"
#include "../Map/TileLayer.h"
#include "../Factory/ObjectFactory.h"
#include "../Graphics/TextureManager.h"
#include "../Collision/CollisionHandler.h"
#include "../Map/ImgLayer.h"
#include "../Parser/Parser.h"

class Play : public GameState{

	public:
		Play();
		void Events();
		virtual bool Init();
		virtual bool Exit();
		virtual void Update();
		virtual void Render();

		void DestroyPlayer();
		void DestroyEnemy(std::shared_ptr<Enemy> enemy);

		void DestroyCollectible(std::shared_ptr<Collectible> collectible);

		template <typename T, typename = std::enable_if_t<std::is_base_of_v<GameObject, T>>>
		std::shared_ptr<T> SpawnGameObjectAtLocation(const GameObjectType type, const Transform& transform) {
			const Properties* props = Parser::GetInstance()->GetGameObjectPropertiesByType(type);
			if (!props) {
				std::cout << "Failed to get properties for GameObject: " << GameObjectTypeToString(type) << std::endl;
				return nullptr;
			}

			auto game_object = ObjectFactory::GetInstance()->CreateGameObject(type, *props, transform);
			if (!game_object) {
				std::cout << "Failed to create GameObject: " << GameObjectTypeToString(type) << std::endl;
				return nullptr;
			}

			m_GameObjects.push_back(game_object);
			return std::dynamic_pointer_cast<T>(game_object);
		}

	private:
		static void OpenMenu();
		static void PauseGame();

		void UpdateCollisionLayers();

		std::pair<int, Tile::Matrix> JoinMatrixOfActiveMaps();

		size_t EstimateMapTotalRows() const;

		TileLayer* GetCollisionLayerFromMap(const std::shared_ptr<TileMap>& map) const;

		void UpdateMaps();

		void AddMapAtPosition(int type, int YPosition);

		void SpawnNewEnemyList(int firstAvailableFloor, std::shared_ptr<TileMap>& map);

		void SpawnNewCollectibleList(int firstAvailableFloor, std::shared_ptr<TileMap>& map);

		template<typename T>
		void SpawnObjectsOnFloors(GameObjectType objectType, int firstAvailableFloor, std::shared_ptr<TileMap>& map, int maxObjectsPerFloor, int verticalOffsetMultiplier = 4, int minObjectsPerMap = -1, int maxObjectsPerMap = -1);

		template<typename T>
		void RemoveOutOfScreenObjects(std::vector<std::shared_ptr<T>>& objectList);

		void RemoveOutOfScreenEnemies();

		void RemoveOutOfScreenCollectibles();


	private:
		bool m_EditMode;
		TileMap* m_LevelMap;
		std::vector<ImgLayer*> m_ParalaxBg;

		std::shared_ptr<Player> PlayerInstance;
		std::vector<std::shared_ptr<Enemy>> EnemyList;
		std::vector<std::shared_ptr<Collectible>> CollectibleList;
		std::vector<std::weak_ptr<GameObject>> m_GameObjects;

		/*std::vector<std::unique_ptr<GameObject>> m_SceneObjects;*/

		std::vector<std::shared_ptr<TileMap>> m_ActiveMaps;
};

