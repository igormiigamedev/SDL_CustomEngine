#include "Camera.h"

Camera* Camera::s_Instance = nullptr;

void Camera::Update(float dt) {
    if (m_Target == nullptr) return;
    
    // Calculates the desired camera position to center the target
    float targetX = m_Target->X - SCREEN_WIDTH / 2;
    float targetY = m_ViewBox.y;

    // Centers the camera if the character is beyond the middle of the screen
    if (m_Target->Y <= SCREEN_HEIGHT / 2) {
        targetY = m_Target->Y - SCREEN_HEIGHT / 2;
    }
    //In an extreme case: If the character is below the camera on the y-axis, reset targetY
    if (m_Target->Y > m_ViewBox.y + SCREEN_HEIGHT) {
        targetY = m_Target->Y - SCREEN_HEIGHT / 2;
    }

    // Limits the desired position within the screen limits
    if (targetX < 0) targetX = 0;
    if (targetY < 0) targetY = 0;
    /*if (targetX > (2 * SCREEN_WIDTH - m_ViewBox.w)) targetX = (2 * SCREEN_WIDTH - m_ViewBox.w);
    if (targetY > (2 * SCREEN_HEIGHT - m_ViewBox.h)) targetY = (2 * SCREEN_HEIGHT - m_ViewBox.h);*/

    if (targetX > (m_SceneWidth - m_ViewBox.w)) targetX = (m_SceneWidth - m_ViewBox.w);
    if (targetY > (m_SceneHeight - m_ViewBox.h)) targetY = (m_SceneHeight - m_ViewBox.h);

    // Linear interpolation to smooth camera transition
    constexpr float lerpFactor = 0.02f; // Adjust this value to control the speed of smoothing
    m_ViewBox.x += (targetX - m_ViewBox.x) * lerpFactor;
    m_ViewBox.y += (targetY - m_ViewBox.y) * lerpFactor;

    // Update camera position
    m_Position = Vector2D(m_ViewBox.x, m_ViewBox.y);
}