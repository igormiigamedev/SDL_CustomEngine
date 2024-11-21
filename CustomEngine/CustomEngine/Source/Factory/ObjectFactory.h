#pragma once
#include <map>
#include <string>
#include <memory>
#include "../Object/GameObject.h"
#include <functional>

enum ObjectCategory {SCENE_OBJECTS = 0, GAME_OBJECTS = 1};

class ObjectFactory{

	public:
		std::unique_ptr<GameObject> CreateGameObject(std::string type, Properties* props);
		void RegisterType(std::string className, std::function<std::unique_ptr<GameObject> (Properties* props)> type);
		static ObjectFactory* GetInstance() { return s_Instance = (s_Instance != nullptr) ? s_Instance : new ObjectFactory(); }

	private:
		ObjectFactory() {}
		static ObjectFactory* s_Instance;
		std::map <std::string, std::function<std::unique_ptr<GameObject> (Properties* props)> > m_TypeRegistry;
};

template<class Type>
class RegisterObject {

	public:
		RegisterObject(std::string className) {
			ObjectFactory::GetInstance()->RegisterType(className, [](Properties* props)->std::unique_ptr<GameObject> {return std::unique_ptr<Type>(new Type(props)); });
		}
};

