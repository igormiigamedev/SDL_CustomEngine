#pragma once

#include <unordered_map>

#include "CollisionTypes.h" // Inclua o cabe�alho com o enum

class CollisionManager {
    private:
        // Mapa de respostas de colis�o
        std::unordered_map<ObjectResponses, CollisionResponses> responseMap;

    public:
        // Construtor para definir os valores iniciais
        CollisionManager() { //BLOCK, IGNORE, OVERLAP
            responseMap[PhysicsBody] = OVERLAP;
            responseMap[WorldFloor] = BLOCK;
            responseMap[WorldWall] = BLOCK;
        }

        // Fun��o para obter a resposta de colis�o
        CollisionResponses GetCollisionResponse(ObjectResponses objResponse) const {
            return responseMap.at(objResponse);
        }

        // Fun��o para configurar a resposta de colis�o
        void SetCollisionResponse(ObjectResponses objResponse, CollisionResponses collisionResponse) {
            responseMap[objResponse] = collisionResponse;
        }
};


