#pragma once

#include "../Object/GameObject.h"
#include <iostream>

class FloorObject : public GameObject {
public:
    FloorObject(const Properties& props, Transform transform)
        : GameObject(props, transform) {
    }

    void InitGameObject() override {
        // Floor does not need to register collision callbacks as it only receives collisions
        std::cout << "FloorObject initialized.\n";
    }

    void Draw() override {
        // The floor may not need to be rendered directly, but may have debug visuals
        std::cout << "FloorObject Draw() called.\n";
    }

    void Clean() override {
        std::cout << "FloorObject cleaned.\n";
    }

    void Update(float dt) override {
        // The ground doesn't move or change in time, so this function can be empty
    }

    GameObjectType GetType() const override {
        return GameObjectType::FLOOR; 
    }

    void OnCollision(std::shared_ptr<GameObject> target) {
        /*std::cout << "Floor collided with " << target->GetType() << std::endl;*/
    }

};


