#include "EGameObjectTypes.h"
#include <stdexcept> 

std::string GameObjectTypeToString(GameObjectType type) {
    switch (type) {
        case GameObjectType::PLAYER: return ENUM_TO_STRING(PLAYER);
        case GameObjectType::ENEMY: return ENUM_TO_STRING(ENEMY);
        default: return "UNKNOWN";
    }
}

GameObjectType StringToGameObjectType(const char* objType) {
    static const std::unordered_map<std::string, GameObjectType> typeMap = {
        {"PLAYER", GameObjectType::PLAYER},
        {"ENEMY", GameObjectType::ENEMY}
    };

    auto it = typeMap.find(objType);
    if (it != typeMap.end()) {
        return it->second;
    }
    //return GameObjectType::UNKNOWN;
    throw std::invalid_argument("Invalid GameObjectType: " + std::string(objType));
}