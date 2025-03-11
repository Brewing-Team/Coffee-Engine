#include "ParticleManager.h"
#include <cstdlib>
#include <ctime>
#include <glm/gtc/random.hpp>




namespace Coffee
{

    // Constructor de Particle
    Particle::Particle() : transformMatrix(glm::mat4(1.0f)), direction(0.0f), color(1.0f), size(1.0f), lifetime(1.0f) {}

    glm::mat4 Particle::GetWorldTransform()
    {
        /*switch (simulationSpace)
        {
            case SimulationSpace::Local:
                return transformMatrix;
            case SimulationSpace::World:
                return glm::mat4(1.0f) * transformMatrix;
            case SimulationSpace::Custom:
                return customTransform * transformMatrix;
        }*/
        return transformMatrix;
    }

    void Particle::SetPosition(glm::vec3 position)
    {
        transformMatrix[3] = glm::vec4(position, 1.0f);
    }

    void Particle::SetRotation(glm::vec3 rotation)
    {
        glm::vec3 position = GetPosition();
        glm::vec3 scale = GetSize();

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, rotation.x, glm::vec3(1, 0, 0));
        rotationMatrix = glm::rotate(rotationMatrix, rotation.y, glm::vec3(0, 1, 0));
        rotationMatrix = glm::rotate(rotationMatrix, rotation.z, glm::vec3(0, 0, 1));

        transformMatrix = glm::scale(rotationMatrix, scale);
        SetPosition(position);
    }

    void Particle::SetSize(glm::vec3 scale)
    {
        glm::vec3 position = GetPosition();
        glm::mat3 rotationMatrix = glm::mat3(glm::normalize(transformMatrix[0]), glm::normalize(transformMatrix[1]),
                                             glm::normalize(transformMatrix[2]));
        transformMatrix = glm::mat4(rotationMatrix);
        transformMatrix = glm::scale(transformMatrix, scale);
        SetPosition(position);
    }

    glm::vec3 Particle::GetPosition()
    {
        return glm::vec3(transformMatrix[3]);
    }

    glm::vec3 Particle::GetRotation()
    {
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


    // Constructor de ParticleEmitter
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
       
        glm::vec3 startPos = glm::linearRand(minSpread, maxSpread);
        glm::vec4 startPosVec4 = glm::vec4(startPos.x, startPos.y, startPos.z, 0);

        glm::mat4 auxMatrix = transformComponentMatrix;
        auxMatrix[3] = transformComponentMatrix[3] + startPosVec4;

        p->transformMatrix = auxMatrix;
        

        p->direction = useDirectionRandom ? glm::linearRand(direction, directionRandom) : direction;
        p->color = useColorRandom ? glm::linearRand(colourNormal, colourRandom) : colourNormal;
        p->lifetime = useRandomLifeTime ? glm::linearRand(startLifeTimeMin, startLifeTimeMax) : startLifeTime;
        p->startLifetime = p->lifetime;

        float startSpeedValue = useRandomSpeed ? glm::linearRand(startSpeedMin, startSpeedMax) : startSpeed;
        p->direction *= startSpeedValue;

        float sizeValue = useRandomSize ? glm::linearRand(startSizeMin, startSizeMax) : startSize;
        p->SetSize(glm::vec3(sizeValue));

        float rotationValue = useRandomRotation ? glm::linearRand(startRotationMin, startRotationMax) : startRotation;
        p->SetRotation(glm::vec3(rotationValue));
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

        if (looping)
        {
            accumulatedParticles += rateOverTime * dt;
            while (accumulatedParticles >= 1.0f && activeParticles.size() < amount)
            {
                GenerateParticle();
                accumulatedParticles -= 1.0f;
            }
        }

        for (size_t i = 0; i < activeParticles.size();)
        {
            UpdateParticle(activeParticles[i], dt);
            //activeParticles[i]->Update(dt);
            if (activeParticles[i]->lifetime <= 0)
            {
                activeParticles.erase(activeParticles.begin() + i);
            }
            else
            {
                ++i;
            }
        }
    }

    void ParticleEmitter::UpdateParticle(Ref<Particle> p, float dt)
    {
        float normalizedLife = 1.0f - (p->lifetime / p->startLifetime);

        float speedFactor = 1.0f + (speedModifier * dt);      
        p->direction *= glm::clamp(speedFactor, 0.1f, 10.0f);

        if (simulationSpace == SimulationSpace::Local)
        {
            p->SetPosition(p->GetPosition() + p->direction * dt);
        }
        else
        {
            p->SetPosition(p->GetPosition() + p->direction * dt);
        }

        if (useSizeOverLifetime)
        {
            glm::vec3 newSize;
            if (separateAxes)
            {
                newSize.x = CurveEditor::GetCurveValue(normalizedLife, sizeOverLifetimeX);
                newSize.y = CurveEditor::GetCurveValue(normalizedLife, sizeOverLifetimeY);
                newSize.z = CurveEditor::GetCurveValue(normalizedLife, sizeOverLifetimeZ);
            }
            else
            {
                float uniformSize = CurveEditor::GetCurveValue(normalizedLife, sizeOverLifetimeGeneral);
                newSize = glm::vec3(uniformSize);
            }
            p->SetSize(newSize);
        }

        if (useRotationOverLifetime)
        {
            glm::vec3 newRotation;

            if (rotationSeparateAxes)
            {
                newRotation.x = rotationOverLifetimeX * normalizedLife;
                newRotation.y = rotationOverLifetimeY * normalizedLife;
                newRotation.z = rotationOverLifetimeZ * normalizedLife;
            }
            else
            {
                newRotation = rotationOverLifetime * normalizedLife;
            }

            if (rotationOverLifetimeAngularVelocity != 0.0f)
            {
                newRotation += glm::vec3(rotationOverLifetimeAngularVelocity * dt);
            }

            p->SetRotation(newRotation);
        }

        p->lifetime -= dt;
    }

} // namespace Coffee
