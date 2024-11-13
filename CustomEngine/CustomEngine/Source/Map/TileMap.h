#pragma once

#include "vector"
#include "Layer.h"

class TileMap{
	public:
		TileMap() {}

		void Render() {
			/*for (unsigned int i = 0; i < m_MapLayers.size(); i++) {
				m_MapLayers[i]->Render();
			}*/
			for (auto layer : m_MapLayers) {
				layer->Render();
			}
		}

		void Update() {
			/*for (unsigned int i = 0; i < m_MapLayers.size(); i++) {
				m_MapLayers[i]->Update();
			}*/
			for (auto layer : m_MapLayers) {
				layer->Update();
			}
		}

		void Clean() {
			for (auto layer : m_MapLayers) {
				delete layer;
			}
			m_MapLayers.clear();
		}

		std::vector<Layer*>& GetMapLayers() { return m_MapLayers; }

	private:
		/*friend class MapParser;*/
		std::vector<Layer*> m_MapLayers;
};

