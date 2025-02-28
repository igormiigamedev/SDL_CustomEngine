#pragma once
#include "../Physics/Transform.h"
#include <string>
#include "../Animation/SpriteAnimation.h"

class ParticleEffects
{
public:
	ParticleEffects();
	void SpawnParticle(Transform transform, std::string animationTxtID, float scaleX, float scaleY, bool loopAnimation);
	void Draw();
	void Update(float dt);
	void Clean();

	bool IsFinished() const { return m_Animation && m_Animation->IsEnded(); }

private:
	Transform m_Transform;
	SpriteAnimation* m_Animation;
	SDL_RendererFlip m_Flip = SDL_FLIP_NONE;
	
	float m_ScaleX, m_ScaleY;
};

