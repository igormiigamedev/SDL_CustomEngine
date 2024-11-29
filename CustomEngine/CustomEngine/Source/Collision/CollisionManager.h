#pragma once

#include <unordered_map>

#include "CollisionTypes.h" // Inclua o cabeçalho com o enum

class CollisionManager {
    private:
        // Mapa de respostas de colisão
        std::unordered_map<ObjectResponses, CollisionResponses> responseMap;

    public:
        // Construtor para definir os valores iniciais
        CollisionManager() { //BLOCK, IGNORE, OVERLAP
            responseMap[PhysicsBody] = OVERLAP;
            responseMap[WorldFloor] = BLOCK;
            responseMap[WorldWall] = BLOCK;
        }

        // Função para obter a resposta de colisão
        CollisionResponses GetCollisionResponse(ObjectResponses objResponse) const {
            return responseMap.at(objResponse);
        }

        // Função para configurar a resposta de colisão
        void SetCollisionResponse(ObjectResponses objResponse, CollisionResponses collisionResponse) {
            responseMap[objResponse] = collisionResponse;
        }
};


