#include "ParticleManager.h"
#include "CoffeeEngine/Scene/Components.h"
#include <cstdlib>
#include <ctime>

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
        p->direction = direction;
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
        timetotal += dt;

        if (looping || elapsedTime < lifeTime)
        {
            while (elapsedTime > rateOverTime)
            {
                GenerateParticle();
                elapsedTime -= rateOverTime;
            }
        }

        for (size_t i = 0; i < activeParticles.size();)
        {
            activeParticles[i]->Update(dt);
            if (activeParticles[i]->lifetime <= 0)
            {
                activeParticles.erase(activeParticles.begin() + i);
            }
            else
            {
                ++i;
            }
        }

        if (!looping && timetotal >= lifeTime )
        {
            // Detener generación de partículas si no es en bucle y ya han expirado
            return;
        }

        printf("Cantidad particulas: %d", (int)activeParticles.size());
    }

    void ParticleEmitter::Render() {}

} // namespace Coffee
