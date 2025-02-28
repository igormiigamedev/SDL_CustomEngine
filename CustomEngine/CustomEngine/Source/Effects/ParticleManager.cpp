#include "ParticleManager.h"
#include "ParticleEffects.h"

ParticleManager* ParticleManager::GetInstance() {
    static ParticleManager instance;
    return &instance;
}

void ParticleManager::AddParticle(std::shared_ptr<ParticleEffects> particle) {
    m_Particles.push_back(particle);
}

void ParticleManager::UpdateParticles(float dt) {
    for (auto it = m_Particles.begin(); it != m_Particles.end(); ) {
        if ((*it)->IsFinished()) {
            it = m_Particles.erase(it);
        }
        else {
            (*it)->Update(dt);
            ++it;
        }
    }
}

void ParticleManager::RenderParticles() {
    for (const auto& particle : m_Particles) {
        particle->Draw();
    }
}

void ParticleManager::ClearParticles() {
    m_Particles.clear();
}
