#include "ParticleManager.h"
#include <cstdlib>

#include "CoffeeEngine/Scene/Components.h"

namespace Coffee
{

    Particle::Particle() : position(0.0f), direction(0.0f), color(1.0f), size(1.0f), lifetime(1.0f) {}


    void Particle::Update(float dt)
    {
        position += direction * dt;
        lifetime -= dt;
    }


    void ParticleEmitter::InitParticle(Ref<Particle> p)
    {
        p->position = glm::vec3(0.0f);
        p->direction = direction; // Ya no es un puntero
        p->color = colourNormal;
        p->size = size;
        p->lifetime = lifeTime;
    }

    void ParticleEmitter::GenerateParticle()
    {
        Ref<Particle> p = CreateRef<Particle>();
        InitParticle(p);
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
            activeParticles[i]->Update(dt);
            if (activeParticles[i]->lifetime <= 0.0f)
            {
                activeParticles.erase(activeParticles.begin() + i);
            }
            else
            {
                ++i;
            }
        }


        printf("Cantidad particulas: %d", activeParticles.size());


    }

    void ParticleEmitter::Render()
    {
        
    }

} // namespace Coffee
