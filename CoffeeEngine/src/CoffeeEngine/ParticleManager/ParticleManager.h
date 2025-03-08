#pragma once
#include "CoffeeEngine/Math/BoundingBox.h"

namespace Coffee
{

    struct Particle
    {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec4 color;
        float size;
        float lifetime;

        Particle();
        void Init(const glm::vec3& startPos);
        void Update(float dt);
    };

    class ParticleEmitter
    {
      private:
        std::vector<Particle> activeParticles;
        float rateOverTime = 0.0f;
        float elapsedTime = 0.0f;

        void GenerateParticle();

      public:
        ParticleEmitter(float rate);
        
        void Update();
        void Render();
    };

} // namespace Coffee