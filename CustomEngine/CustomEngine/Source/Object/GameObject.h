#pragma once

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "IObject.h"
#include "../Physics/Transform.h"
#include <SDL.h>
#include"../Physics/Point.h"
#include "EGameObjectTypes.h"

struct Properties {

    public:
        Properties(int width, int height, float scaleX = 1.0f, float scaleY = 1.0f) {
            Width = width;
            Height = height;
            ScaleX = scaleX;
            ScaleY = scaleY;
        }

    public:
        int Width, Height;
        float ScaleX, ScaleY;
};

class GameObject : public IObject {
    public:
        GameObject(const Properties& props, Transform transform)
            : m_Properties(props), m_Transform(transform) {

            float px = m_Transform.X + props.Width / 2;
            float py = m_Transform.Y + props.Height / 2;
            m_Origin = new Point(px, py);
        }

        inline Point* GetOrigin() { return m_Origin; }

        virtual void Draw() = 0;
        virtual void Clean() = 0;
        virtual void Update(float dt) = 0;

        virtual ~GameObject() {
            /*delete m_Transform;*/ // Libera a memória alocada
        }

    protected:
        Point* m_Origin;
        Transform m_Transform;
        /*int m_SpriteSheetWidth, m_SpriteSheetHeight;*/
        std::string m_TextureID;
        SDL_RendererFlip m_Flip = SDL_FLIP_NONE;
        Properties m_Properties;
};

#endif // GAMEOBJECT_H


