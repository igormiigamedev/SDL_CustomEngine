#pragma once
#include <vector>
#include <memory>

// Forward declaration to avoid circular dependency
class ParticleEffects;

class ParticleManager {
public:
    static ParticleManager* GetInstance();

    void AddParticle(std::shared_ptr<ParticleEffects> particle);
    void UpdateParticles(float dt);
    void RenderParticles();
    void ClearParticles();

private:
    ParticleManager() = default;
    std::vector<std::shared_ptr<ParticleEffects>> m_Particles;
};

