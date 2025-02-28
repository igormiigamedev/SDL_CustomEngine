#include "SpriteAnimation.h"
#include "../Graphics/TextureManager.h"
#include "../Vendor/tinyXML/tinyxml.h"

SpriteAnimation::SpriteAnimation(bool repeat) :Animation(repeat) {}

void SpriteAnimation::Update(float dt){
	if (m_IsEnded) return; // Se a animação terminou, não atualiza mais

	//m_SpriteFrame = (SDL_GetTicks() / m_CurrentSpriteSheet.Speed) % m_CurrentSpriteSheet.FrameCount;

	// If not to loop, check if the animation has finished
	//if (!m_Loop && m_SpriteFrame >= m_CurrentSpriteSheet.FrameCount - 1) {
	//	m_SpriteFrame = m_CurrentSpriteSheet.FrameCount - 1; // Keep on last frame
	//	m_IsEnded = true; // Mark the animation as finished
	//}

	// Accumulate past time
	m_AccumulatedTime += dt;
	float FrameDuration = m_CurrentSpriteSheet.Speed / 1000.0f;

	// Check if the time required to change the frame has passed
	if (m_AccumulatedTime >= FrameDuration) {
		m_AccumulatedTime -= FrameDuration; // Reset the accumulated

		// Update the frame
		m_SpriteFrame = (m_SpriteFrame + 1) % m_CurrentSpriteSheet.FrameCount;

		// If not to loop, check if the animation has finished
		if (!m_Loop && m_SpriteFrame >= m_CurrentSpriteSheet.FrameCount - 1) {
			m_SpriteFrame = m_CurrentSpriteSheet.FrameCount - 1; // Keep on last frame
			m_IsEnded = true; // Mark the animation as finished
		}
	}
		
}

void SpriteAnimation::Draw(float x, float y, float scaleX, float scaleY, SDL_RendererFlip flip){

	if (!m_Loop && m_IsEnded) return; // If it's not in a loop and it's already finished, don't draw anymore

	int currentColumn = (m_SpriteFrame % m_CurrentSpriteSheet.ColCount) + 1;
	int currentRow = (m_SpriteFrame/m_CurrentSpriteSheet.ColCount) + 1;

	TextureManager::GetInstance()->DrawFrame(m_CurrentSpriteSheet.TextureID, x, y, GetCurrentSpriteWidth(), GetCurrentSpriteHeight(), currentRow, currentColumn, flip, scaleX, scaleY);
}

void SpriteAnimation::Parse() {

	std::string source = "Assets/GameAnimations.xml";

	TiXmlDocument xml;
	xml.LoadFile(source);
	if (xml.Error()) {
		std::cout << "Failed to load animation file: " << source << std::endl;
	}

	TiXmlElement* root = xml.RootElement();
	for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
		if (e->Value() == std::string("spriteSheet")) {
			SpriteSheet spriteSheet;
			std::string spriteSheetID = e->Attribute("id");
			spriteSheet.TextureID = e->Attribute("id");
			e->Attribute("frameCount", &spriteSheet.FrameCount);
			e->Attribute("rowCount", &spriteSheet.RowCount);
			e->Attribute("colCount", &spriteSheet.ColCount);
			e->Attribute("speed", &spriteSheet.Speed);
			e->Attribute("width", &spriteSheet.Width);
			e->Attribute("height", &spriteSheet.Height);

			m_SpriteSheetMap[spriteSheetID] = spriteSheet;
		}
	}
}

void SpriteAnimation::SetAnimation(const std::string& textureID, int animSpeed, bool loop) {

	if (!IsValidTexture(textureID)) { return; }

	m_CurrentSpriteSheet = m_SpriteSheetMap[textureID];

	if (animSpeed != 0) {
		m_CurrentSpriteSheet.Speed = animSpeed;
	}

	m_Loop = loop;
}

bool SpriteAnimation::IsValidTexture(const std::string& textureID) {
	auto it = m_SpriteSheetMap.find(textureID);
	if (it == m_SpriteSheetMap.end()) {
		HandleInvalidTexture(textureID);
		return false;
	}
	return true;
}

void SpriteAnimation::HandleInvalidTexture(const std::string& textureID) {
	std::cerr << "Error: The texture '" << textureID << "' was not found in sprite map." << std::endl;

	// Tenta usar uma textura padrão, se disponível
	if (m_SpriteSheetMap.find("default") != m_SpriteSheetMap.end()) {
		m_CurrentSpriteSheet = m_SpriteSheetMap["default"];
		std::cerr << "Warning: Using the default texture." << std::endl;
	}
	else {
		std::cerr << "Warning: No default texture defined. The sprite may not display correctly." << std::endl;
	}
}
