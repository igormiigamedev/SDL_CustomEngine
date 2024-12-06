#pragma once

#include "Collider.h"

class RectCollider : public Collider {
public:
    RectCollider(int x, int y, int w, int h)
        : m_Box({ x, y, w, h}) {}


    void DrawDebugCollider() override {
        Vector2D cam = Camera::GetInstance()->GetPosition();
        SDL_Rect debugBox = m_Box;
        /*debugBox.x -= cam.X;*/
        debugBox.y -= cam.Y;
        SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &debugBox);
    }

    void SetPositionX(float x) override { m_Box.x = static_cast<int>(x) - (m_Box.w / 2); }
    void SetPositionY(float y) override { m_Box.y = static_cast<int>(y) - (m_Box.h / 2); }

    SDL_Rect &GetBox() { return m_Box; }
    const SDL_Rect& GetBox() const { return m_Box; }
    ColliderType GetType() const override { return RECTANGLE; }

    inline float GetCenterPositionX() { return m_Box.x + (m_Box.w / 2); }
    inline float GetCenterPositionY() { return m_Box.y + (m_Box.h / 2); }
    inline float GetWeight() { return m_Box.w; }
    inline float GetHeigth() { return m_Box.h; }

private:
    SDL_Rect m_Box;
};
