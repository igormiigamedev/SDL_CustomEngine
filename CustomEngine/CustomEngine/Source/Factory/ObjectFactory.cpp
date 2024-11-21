#include "ObjectFactory.h"
#include "../Characters/Enemy.h"
#include "../Characters/Player.h"

ObjectFactory* ObjectFactory::s_Instance = nullptr;

std::unique_ptr<GameObject> ObjectFactory::CreateGameObject(std::string type, Properties* props){

	std::unique_ptr<GameObject> object = nullptr;
	auto it = m_TypeRegistry.find(type);

	if (it != m_TypeRegistry.end()) {
		object = it->second(props);
	}

	return object;
}

void ObjectFactory::RegisterType(std::string className, std::function<std::unique_ptr<GameObject> (Properties* props)> type) {
	m_TypeRegistry[className] = type;
}

