#include "ParticleEffects.h"
#include "ParticleManager.h"

ParticleEffects::ParticleEffects(){
}

void ParticleEffects::SpawnParticle(Transform transform, std::string animationTxtID, float scaleX, float scaleY, bool loopAnimation){
    m_Animation = new SpriteAnimation();
    m_Animation->Parse();
    m_Flip = SDL_FLIP_HORIZONTAL;
    m_Animation->SetAnimation(animationTxtID, 0, loopAnimation);

    m_Transform = transform;
    m_ScaleX = scaleX;
    m_ScaleY = scaleY;

    ParticleManager::GetInstance()->AddParticle(std::make_shared<ParticleEffects>(*this));
}

void ParticleEffects::Draw(){
    if (! m_Animation->IsEnded()) {
        m_Animation->Draw(m_Transform.X, m_Transform.Y, m_ScaleX, m_ScaleY, m_Flip);
    }
}

void ParticleEffects::Update(float dt){
    m_Animation->Update(dt);
}

void ParticleEffects::Clean(){
}
