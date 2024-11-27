#pragma once
#include "../Physics/Transform.h"

class Layer
{
	public:
		virtual void Render()=0;
		virtual void Update() = 0;
		void SetLayerPosition(Transform pos) { m_LayerWorldPosition = pos; }
		Transform GetLayerPosition() { return m_LayerWorldPosition; }

	protected:
		Transform m_LayerWorldPosition;
};

