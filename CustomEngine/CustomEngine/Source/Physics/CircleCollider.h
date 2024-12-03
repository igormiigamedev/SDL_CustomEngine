#pragma once

#include "Collider.h"
#include "../Physics/Circle.h"

class CircleCollider : public Collider {
public:
    CircleCollider(float x, float y, float radius)
        : m_Circle({ x, y, radius }) {}

    void DrawDebugCollider() override {
        const Vector2D& cameraPosition = Camera::GetInstance()->GetPosition();
        const float screenX = m_Circle.x /*- cameraPosition.X*/;
        const float screenY = m_Circle.y - cameraPosition.Y;

        float innerRadius = m_Circle.radius * 0.9f;  // The size of the empty circle will be x% of the total radius
        const float outerRadiusSquared = m_Circle.radius * m_Circle.radius;
        const float innerRadiusSquared = innerRadius * innerRadius;  

        for (float width = 0; width < m_Circle.radius * 2; ++width) {
            for (float height = 0; height < m_Circle.radius * 2; ++height) {
                // Calculate the distance squared from the center of the circle
                const float dx = m_Circle.radius - width;
                const float dy = m_Circle.radius - height;
                const float distanceSquared = dx * dx + dy * dy;

                // Check if the point is within the outer circle but outside the inner circle
                if (distanceSquared <= outerRadiusSquared && distanceSquared >= innerRadiusSquared) {
                    SDL_RenderDrawPoint(Engine::GetInstance()->GetRenderer(), static_cast<int>(screenX + dx), static_cast<int>(screenY + dy));
                }
            }
        }
    }

    void SetPositionX(float x) override { m_Circle.x = x; }
    void SetPositionY(float y) override { m_Circle.y = y; }

    ColliderType GetType() const override { return CIRCLE; }

    Circle GetCircle() const { return m_Circle; }
    inline float GetCenterPositionX() { return m_Circle.x;}
    inline float GetCenterPositionY() { return m_Circle.y;}
    inline float GetWeight() { return m_Circle.radius * 2; }
    inline float GetHeigth() { return m_Circle.radius * 2; }

private:
    Circle m_Circle;
};
