#include "Camera.h"

Camera* Camera::s_Instance = nullptr;

void Camera::Update(float dt) {
    if (m_Target == nullptr) return;
    
    const float screenUpperLimitFactor = SCREEN_HEIGHT * 0.4f;  // Represents x% of the screen height
    const float screenLowerLimitFactor = SCREEN_HEIGHT * 0.71f; // Represents x% of the screen height

    // Calculates the upper and lower limits of the viewing area
    const float upperLimit = m_ViewBox.y + screenUpperLimitFactor;
    const float lowerLimit = m_ViewBox.y + screenLowerLimitFactor;

    // Calculates the desired camera position to center the target
    /*float targetX = m_Target->X - SCREEN_WIDTH / 2;*/
    float newViewboxY = m_ViewBox.y;
    /*std::cout << "m_Target->Y - newViewboxY: " << m_Target->Y - newViewboxY << std::endl;*/

    // Centers the camera if the character is beyond the middle of the screen
    if ( m_Target->Y <= upperLimit) {
        newViewboxY = m_ViewBox.y - (screenUpperLimitFactor);
    }
    //In an extreme case: If the character is below the camera on the y-axis, reset targetY
    if (m_Target->Y >= lowerLimit) {
        newViewboxY = m_ViewBox.y + (screenLowerLimitFactor);
    }

    // Limits the desired position within the screen limits
    /*if (targetX < 0) targetX = 0;*/
    /*if (targetY < 0) targetY = 0;*/

    /*if (targetX > (2 * SCREEN_WIDTH - m_ViewBox.w)) targetX = (2 * SCREEN_WIDTH - m_ViewBox.w);
    if (targetY > (2 * SCREEN_HEIGHT - m_ViewBox.h)) targetY = (2 * SCREEN_HEIGHT - m_ViewBox.h);*/

    /*if (targetX > (m_SceneWidth - m_ViewBox.w)) targetX = (m_SceneWidth - m_ViewBox.w);*/
    /*if (targetY > (m_SceneHeight - m_ViewBox.h)) targetY = (m_SceneHeight - m_ViewBox.h);*/

    // Linear interpolation to smooth camera transition
    constexpr float lerpFactor = 0.011f; // Adjust this value to control the speed of smoothing
    /*m_ViewBox.x += (targetX - m_ViewBox.x) * lerpFactor;*/
    m_ViewBox.y += (newViewboxY - m_ViewBox.y) * lerpFactor;

    // Update camera position
    m_Position = Vector2D(m_ViewBox.x, m_ViewBox.y);
}