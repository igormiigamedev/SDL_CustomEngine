#pragma once

#include <vector>
#include <memory>
#include "Layer.h"
#include "../Physics/Transform.h"

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

    void Render() {
        for (const auto& layer : m_MapLayers) {
            layer->Render();
        }
    }

    void Update() {
        for (const auto& layer : m_MapLayers) {
            layer->Update();
        }
    }

    void Clean() {
        m_MapLayers.clear(); 
    }

    void SetPosition(int x, int y) {
        Transform tempPos;
        tempPos.X = x;
        tempPos.Y = y;
        for (auto& layer : m_MapLayers) {
            layer->SetLayerPosition(tempPos);
        }
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

    std::vector<std::unique_ptr<Layer>>& GetMapLayers() { return m_MapLayers; }

private:
    std::vector<std::unique_ptr<Layer>> m_MapLayers;
    int width{}, height{};
};
