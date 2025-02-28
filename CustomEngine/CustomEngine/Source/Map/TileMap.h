#pragma once

#include <vector>
#include <memory>
#include "TileLayer.h"
#include "../Physics/Transform.h"
#include "../Floor/FloorObject.h"
#include "../Physics/Collider.h"
#include "../Physics/RectCollider.h"
#include "../Physics/CircleCollider.h"
#include "../Collision/PhysicsWorld.h"
#include "../Collision/CollisionTypes.h"

class TileMap {
public:
    TileMap() = default;
    ~TileMap() = default;

    // Copy constructor (deep copy)
    TileMap(const TileMap& other) {
        width = other.width;
        height = other.height;
        for (const auto& layer : other.m_MapLayers) {
            m_MapLayers.push_back(std::unique_ptr<Layer>(layer->Clone()));
        }
        dynamic_cast<TileLayer*>(GetMapCollisionLayer().get())->UpdateFloorCollisionInformations();
        m_FloorGameObject = std::make_shared<FloorObject>(Properties(), Transform());
    }

    TileMap& operator=(const TileMap& other) {
        if (this == &other) return *this;
        m_MapLayers.clear();
        width = other.width;
        height = other.height;
        for (const auto& layer : other.m_MapLayers) {
            m_MapLayers.push_back(std::unique_ptr<Layer>(layer->Clone()));
        }
        return *this;
    }

    void SetCollider(std::shared_ptr<Collider> collider) {
        m_ColliderList.push_back(std::move(collider));
        m_ColliderList.back()->SetOwner(m_FloorGameObject);
        PhysicsWorld::GetInstance()->RegisterCollider(m_ColliderList.back());
        m_ColliderList.back()->SetCollisionResponse(WorldFloor, IGNORE);
        m_ColliderList.back()->SetCollisionResponse(PhysicsBody, IGNORE);
    }

    void SetMapColliders() {
        Transform transform;

        TileLayer* collisionLayer = dynamic_cast<TileLayer*>(GetMapCollisionLayer().get());
        
        for (int floor = 1; floor <= collisionLayer->GetAmountOfFloorCollision(); floor++) {
            transform = collisionLayer->GetFloorCenterPosition(floor);
            int floorHeight = collisionLayer->GetFloorSize(floor);
            if (floorHeight == collisionLayer->GetTileSize()) {
                floorHeight = floorHeight + collisionLayer->GetTileSize() / 2;
            }
            transform.Y = transform.Y - floorHeight/2;
            Transform mapPosition = collisionLayer->GetLayerPosition();

            SetCollider(std::make_shared<RectCollider>(0, mapPosition.Y + transform.Y, collisionLayer->GetTileWidth() * collisionLayer->GetTileSize(), floorHeight, WorldFloor));
        }
    }

    void Render() {
        for (const auto& layer : m_MapLayers) {
            layer->Render();
        }
        for (const auto& collider : m_ColliderList) {
            collider->DrawDebugCollider();
        }
    }

    void Update() {
        for (const auto& layer : m_MapLayers) {
            layer->Update();
        }
    }

    void Clean() {
        m_ColliderList.clear();

        m_MapLayers.clear();

        m_FloorGameObject.reset();

        std::cout << "TileMap cleaned.\n";
    }


    void SetPosition(int x, int y) {
        Transform tempPos;
        tempPos.X = x;
        tempPos.Y = y;
        for (auto& layer : m_MapLayers) {
            layer->SetLayerPosition(tempPos);
        }
        SetMapColliders();
    }

    Transform GetPosition() {
        return m_MapLayers.back()->GetLayerPosition();
    }

    void SetSize(int h, int w) {
        height = h;
        width = w;
    }

    int GetHeight() {
        return height;
    }

    std::unique_ptr<Layer>& GetMapCollisionLayer() { return m_MapLayers.back(); }

    std::vector<std::unique_ptr<Layer>>& GetMapLayers() { return m_MapLayers; }

private:
    std::vector<std::unique_ptr<Layer>> m_MapLayers;
    int width{}, height{};

    std::vector < std::shared_ptr<Collider>> m_ColliderList;
    std::shared_ptr<GameObject> m_FloorGameObject;
};
