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

class Play : public GameState{

	public:
		Play();
		void Events();
		virtual bool Init();
		virtual bool Exit();
		virtual void Update();
		virtual void Render();

		template< typename T, typename = std::enable_if_t< std::is_base_of_v< GameObject, T > > >
		T* SpawnGameObject(std::string type, Properties* props) {
			/*T* game_object = new T(props);*/
			T* game_object = ObjectFactory::GetInstance()->CreateObject(type, props);
			m_GameObjects.push_back(game_object);
			return game_object;
		}

	private:
		static void OpenMenu();
		static void PauseGame();

	private:
		bool m_EditMode;
		TileMap* m_LevelMap;
		std::vector<GameObject*> m_GameObjects;
		std::vector<ImgLayer*> m_ParalaxBg;
};

