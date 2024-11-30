#include "ObjectFactory.h"
#include "../Characters/Enemy.h"
#include "../Characters/Player.h"

ObjectFactory* ObjectFactory::s_Instance = nullptr;

std::shared_ptr<GameObject> ObjectFactory::CreateGameObject(GameObjectType type, const Properties& props, Transform transform){

	std::shared_ptr<GameObject> object = nullptr;
	auto it = m_TypeRegistry.find(type);

	if (it != m_TypeRegistry.end()) {
		object = it->second(props, transform);
	}

	return object;
}

void ObjectFactory::RegisterType(GameObjectType className, std::function<std::shared_ptr<GameObject>(const Properties& props, Transform transform)> type) {
	m_TypeRegistry[className] = type;
}

