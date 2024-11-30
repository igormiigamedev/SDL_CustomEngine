#pragma once
#include <map>
#include <string>
#include <memory>
#include "../Object/GameObject.h"
#include <functional>

enum ObjectCategory {SCENE_OBJECTS = 0, GAME_OBJECTS = 1};

class ObjectFactory{

	public:
		std::shared_ptr<GameObject> CreateGameObject(GameObjectType type, const Properties& props, Transform transform);
		void RegisterType(GameObjectType className, std::function<std::shared_ptr<GameObject>(const Properties& props, Transform transform)> type);
		static ObjectFactory* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new ObjectFactory(); }

	private:
		ObjectFactory() {}
		static ObjectFactory* s_Instance;
		std::map <GameObjectType, std::function<std::shared_ptr<GameObject>(const Properties& props, Transform transform)> > m_TypeRegistry;
};

template<class Type>
class RegisterObject {

	public:
		RegisterObject(GameObjectType className) {
			ObjectFactory::GetInstance()->RegisterType(className, [](const Properties& props, Transform transform)->std::shared_ptr<GameObject> {return  std::make_shared<Type>(props, transform); }); //std::shared_ptr<Type>(new Type(props, transform)); });
		}
};

