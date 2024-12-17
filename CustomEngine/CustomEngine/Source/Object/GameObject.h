#pragma once

#include "IObject.h"
#include "../Physics/Transform.h"
#include <SDL.h>
#include "EGameObjectTypes.h"
#include <memory>

struct Properties {

    public:
        Properties(float scaleX = 1.0f, float scaleY = 1.0f) {
            ScaleX = scaleX;
            ScaleY = scaleY;
        }

    public:
        float ScaleX, ScaleY;
};

class GameObject : public IObject, public std::enable_shared_from_this<GameObject> {
    public:
        GameObject(const Properties& props, Transform transform)
            : m_Properties(props), m_Transform(transform) {
        }

        virtual void Draw() = 0;
        virtual void Clean() = 0;
        virtual void Update(float dt) = 0;

        virtual GameObjectType GetType() const = 0;

        virtual ~GameObject() {
            /*delete m_Transform;*/ // Libera a memória alocada
        }

        std::weak_ptr<GameObject> GetWeakPtr() {
            return shared_from_this();
        }

        virtual void InitGameObject() = 0;

        inline const Transform& GetTransform() const {
            return m_Transform;
        }

    protected:
        Transform m_Transform;
        SDL_RendererFlip m_Flip = SDL_FLIP_NONE;
        Properties m_Properties;
};


