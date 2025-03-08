#include "ParticleManager.h"
#include <cstdlib>

namespace Coffee
{

    Particle::Particle() : position(0.0f), velocity(0.0f), color(1.0f), size(1.0f), lifetime(1.0f) {}

    void Particle::Init(const glm::vec3& startPos)
    {
        position = startPos;
        velocity = glm::vec3((rand() % 100 - 50) / 50.0f, 1.0f, (rand() % 100 - 50) / 50.0f);
        color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        size = 0.1f + static_cast<float>(rand()) / RAND_MAX * 0.2f;
        lifetime = 2.0f;
    }

    void Particle::Update(float dt)
    {
        position += velocity * dt;
        lifetime -= dt;
    }

    

    void ParticleEmitter::GenerateParticle()
    {
        Particle p;
        p.Init(glm::vec3(0.0f)); 
        activeParticles.push_back(p);
    }

    void ParticleEmitter::Update()
    {
        float dt = 0.16f;
        elapsedTime += dt;

        while (elapsedTime > rateOverTime)
        {
            GenerateParticle();
            elapsedTime -= rateOverTime;
        }

        for (size_t i = 0; i < activeParticles.size();)
        {
            activeParticles[i].Update(dt);
            if (activeParticles[i].lifetime <= 0.0f)
            {
                activeParticles.erase(activeParticles.begin() + i);
            }
            else
            {
                ++i;
            }
        }
    }

    void ParticleEmitter::Render()
    {
        
    }

} // namespace Coffee
