#include "Camera.h"

Camera* Camera::s_Instance = nullptr;

void Camera::Update(float dt){

	if (m_Target != nullptr) {
		m_ViewBox.x = m_Target->X - SCREEN_WIDTH / 2;

		//If the character goes beyond the middle of the screen, re-center the camera
		if (m_Target->Y <= SCREEN_HEIGHT / 2) {
			m_ViewBox.y = m_Target->Y - SCREEN_HEIGHT / 2;
		}
		
		//In an extreme case: If the player moves down the screen and goes to a point on the y-axis below the camera
		if (m_Target->Y > m_ViewBox.y + SCREEN_HEIGHT) {
			m_ViewBox.y = m_Target->Y - SCREEN_HEIGHT / 2;
		}

		if (m_ViewBox.x < 0) {
			m_ViewBox.x = 0;
		}

		if (m_ViewBox.y < 0) {
			m_ViewBox.y = 0;
		}

		if (m_ViewBox.x >  (2 * SCREEN_WIDTH - m_ViewBox.w)) {
			m_ViewBox.x = (2 * SCREEN_WIDTH - m_ViewBox.w);
		}

		if (m_ViewBox.y > (2 * SCREEN_HEIGHT - m_ViewBox.h)) {
			m_ViewBox.y = (2 * SCREEN_HEIGHT - m_ViewBox.h);
		}

		m_Position = Vector2D(m_ViewBox.x, m_ViewBox.y);
	}
}
