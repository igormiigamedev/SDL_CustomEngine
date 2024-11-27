#pragma once

#include <iostream>
#include <string>
#include "Layer.h"
#include "SDL.h"
#include "SDL_image.h"

class ImgLayer : public Layer{

	public:
		ImgLayer(std::string textureID, int x, int y, float scrollRatio = 1.0f, float scaleX = 1.0f, float scaleY = 1.0f);
		void SetScrollRatio(float scrollRatio) { m_ScrollRatio = scrollRatio; }
		void SetOffset(int x, int y) { m_OffsetX = x; m_OffsetY = y; }
		void SetRepeated(bool repeated) { m_Repeated = repeated; }
		void QueryImage(std::string textureID);

		virtual void Render();
		virtual void Update();

		std::unique_ptr<Layer> Clone() const override {
			return std::make_unique<ImgLayer>(*this);
		}

	private:
		bool m_Repeated;
		float m_ScrollRatio;
		std::string m_TextureID;
		float m_ScaleX, m_ScaleY;
		int m_OffsetX, m_OffsetY;
		int m_ImgWidth, m_ImgHeight;
		SDL_RendererFlip m_Flip;
};

