#include "ImgLayer.h"
#include "../Graphics/TextureManager.h"

ImgLayer::ImgLayer(std::string textureID, int x, int y, float scrollRatio, float scaleX, float scaleY){
	m_TextureID = textureID;
	m_OffsetX = x;
	m_OffsetY = y;
	m_ScrollRatio = scrollRatio;
	m_Repeated = true;
	m_ScaleX = scaleX;
	m_ScaleY = scaleY;
	QueryImage(m_TextureID);
}

void ImgLayer::QueryImage(std::string textureID){
	SDL_Texture* texture = TextureManager::GetInstance()->GetTexture(textureID);
	SDL_QueryTexture(texture, NULL, NULL, &m_ImgHeight, &m_ImgWidth);
}

void ImgLayer::Render(){
	TextureManager::GetInstance()->Draw(m_TextureID, m_OffsetX, m_OffsetY, m_ImgWidth, m_ImgHeight, m_ScaleX, m_ScaleY, m_ScrollRatio, m_Flip);
}

void ImgLayer::Update(){
}
