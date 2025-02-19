#pragma once
#include <unordered_map>
#include <iostream>
#include <string>

#define ENUM_TO_STRING(e) #e

enum class GameObjectType { PLAYER, ENEMY, COLLECTIBLE, FLOOR, UNKNOWN};

std::string GameObjectTypeToString(GameObjectType type);

GameObjectType StringToGameObjectType(const char* objType);


