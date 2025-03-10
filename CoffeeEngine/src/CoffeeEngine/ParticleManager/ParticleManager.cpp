#include "ParticleManager.h"
#include <cstdlib>
#include <ctime>

namespace Coffee
{

    Particle::Particle() : transformMatrix(glm::mat4(1.0f)), direction(0.0f), color(1.0f), size(1.0f), lifetime(1.0f) {}

    void Particle::Update(float dt)
    {
        // position += direction * dt;
        SetPosition(GetPosition() + direction * dt);
        lifetime -= dt;
    }

    glm::mat4 Particle::GetWorldTransform()
    {
        return transformMatrix;
    }

    void Particle::SetPosition(glm::vec3 position)
    {
        transformMatrix[3] = glm::vec4(position, 1.0f);
    }

    void Particle::SetRotation(glm::vec3 rotation)
    {
        transformMatrix = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0));
        transformMatrix = glm::rotate(transformMatrix, rotation.y, glm::vec3(0, 1, 0));
        transformMatrix = glm::rotate(transformMatrix, rotation.z, glm::vec3(0, 0, 1));
    }

    void Particle::SetSize(glm::vec3 scale)
    {
        transformMatrix = glm::scale(glm::mat4(1.0f), scale);
    }

    glm::vec3 Particle::GetPosition()
    {
        return glm::vec3(transformMatrix[3]);
    }

    glm::vec3 Particle::GetRotation()
    {
        // Extraer rotación de la matriz de transformación (simplificado)
        return glm::vec3(atan2(transformMatrix[1][2], transformMatrix[2][2]),
                         atan2(-transformMatrix[0][2], sqrt(transformMatrix[1][2] * transformMatrix[1][2] +
                                                            transformMatrix[2][2] * transformMatrix[2][2])),
                         atan2(transformMatrix[0][1], transformMatrix[0][0]));
    }

    glm::vec3 Particle::GetSize()
    {
        return glm::vec3(glm::length(glm::vec3(transformMatrix[0])), glm::length(glm::vec3(transformMatrix[1])),
                         glm::length(glm::vec3(transformMatrix[2])));
    }

    Ref<Mesh> ParticleEmitter::particleMesh = nullptr;
    ParticleEmitter::ParticleEmitter()
    {
        if (!particleMesh)
        {
            particleMesh = Coffee::PrimitiveMesh::CreateQuad();
        }
    }

    void ParticleEmitter::InitParticle(Ref<Particle> p)
    {
        p->transformMatrix = glm::mat4(1.0f);
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

    void ParticleEmitter::Update(float dt)
    {

        elapsedTime += dt;

        while (elapsedTime > rateOverTime)
        {
            GenerateParticle();
            elapsedTime -= rateOverTime;
        }

        for (size_t i = 0; i < activeParticles.size();)
        {
            activeParticles[i]->Update(dt);
            if (activeParticles.size() > amount)
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

} // namespace Coffee
