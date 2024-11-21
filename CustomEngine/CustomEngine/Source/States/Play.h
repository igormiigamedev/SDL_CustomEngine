#pragma once
#include <iostream>
#include "../Object/GameObject.h"
#include "GameState.h"
#include "../Characters/Enemy.h"
#include "../Characters/Player.h"
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

		template <typename T, typename = std::enable_if_t<std::is_base_of_v<GameObject, T>>>
		T* SpawnGameObjectAtLocation(const GameObjectType type, const Transform& transform) {
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
			m_GameObjects.push_back(std::move(game_object));
			return static_cast<T*>(m_GameObjects.back().get());
		}

	private:
		static void OpenMenu();
		static void PauseGame();

	private:
		bool m_EditMode;
		TileMap* m_LevelMap;
		std::vector<ImgLayer*> m_ParalaxBg;

		std::vector<std::unique_ptr<GameObject>> m_GameObjects;
		std::vector<std::unique_ptr<GameObject>> m_SceneObjects;
};

