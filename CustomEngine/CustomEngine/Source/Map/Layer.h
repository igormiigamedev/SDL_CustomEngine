#pragma once
#include "../Physics/Transform.h"

class Layer {
public:
    virtual ~Layer() = default;

    virtual void Render() = 0;
    virtual void Update() = 0;

    void SetLayerPosition(Transform pos) { m_LayerWorldPosition = pos; }
    Transform GetLayerPosition() { return m_LayerWorldPosition; }

    virtual std::unique_ptr<Layer> Clone() const = 0; // Deep Copy Method

protected:
    Transform m_LayerWorldPosition;
};
