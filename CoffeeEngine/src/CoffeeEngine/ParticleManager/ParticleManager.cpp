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
        // Extraer la posición y escala actuales
        glm::vec3 position = GetPosition();
        glm::vec3 scale = GetSize();

        // Crear una nueva matriz solo con la rotación
        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(1, 0, 0));
        rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0, 1, 0));
        rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0, 0, 1));

        // Reconstruir la matriz con rotación y escala
        transformMatrix = glm::scale(rotationMatrix, scale);

        // Restaurar la posición
        SetPosition(position);
    }

    void Particle::SetSize(glm::vec3 scale)
    {
        // Extraer la posición actual
        glm::vec3 position = GetPosition();

        // Extraer la rotación SIN la escala previa
        glm::mat3 rotationMatrix = glm::mat3(glm::normalize(transformMatrix[0]), glm::normalize(transformMatrix[1]),
                                             glm::normalize(transformMatrix[2]));

        // Construir una nueva matriz con la rotación limpia y la nueva escala
        transformMatrix = glm::mat4(rotationMatrix);          // Mantiene la rotación sin escala
        transformMatrix = glm::scale(transformMatrix, scale); // Aplica la nueva escala

        // Restaurar la posición
        SetPosition(position);
    }

    glm::vec3 Particle::GetPosition()
    {
        return glm::vec3(transformMatrix[3]);
    }

    glm::vec3 Particle::GetRotation()
    {
        // Extraer rotaci�n de la matriz de transformaci�n (simplificado)
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
        timetotal += dt;

        if (looping || elapsedTime < lifeTime)
        {
            while (elapsedTime > rateOverTime)
            {
                if (activeParticles.size() < amount)
                {
                    GenerateParticle();
                }
                
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
            // Detener generaci�n de part�culas si no es en bucle y ya han expirado
            return;
        }

        printf("Cantidad particulas: %d", (int)activeParticles.size());
    }

   


} // namespace Coffee
