#pragma once
#include "CoffeeEngine/Math/BoundingBox.h"
#include "CoffeeEngine/Core/Base.h"


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
        //void Init();
        void Update(float dt);


         template <class Archive> void serialize(Archive& archive)
        {
            archive(position, velocity, color, size, lifetime);
        }

    };



    struct ParticlesSystemComponent;
    class ParticleEmitter
    {

        public:
        glm::vec3 velocity = {0.0f, 1.0f, 0.0f};
        glm::vec3 spread = {1.0f, 1.0f, 1.0f};
        glm::vec4 colour = {1.0f, 1.0f, 1.0f, 1.0f};
        float lifeTime = 5.0f;
        float size = 1.0f;
        int amount = 100;
        int textureID = -1; // Placeholder for texture handling

        //Looping
        bool looping = false;

        // Start Life Time
        bool useRandomLifeTime = false; 
        float startLifeTimeMin = 5.0f;
        float startLifeTimeMax = 5.0f;
        float startLifeTime = 5.0f;

        // Start Speed
        bool useRandomSpeed = false;
        float startSpeedMin = 5.0f;
        float startSpeedMax = 5.0f;
        float startSpeed = 5.0f;


          // Start Size
        bool useRandomSize = false;
        float startSizeMin = 5.0f;
        float startSizeMax = 5.0f;
        float startSize = 5.0f;

      private:
        std::vector<Ref<Particle>> activeParticles;
        float rateOverTime = 1.0f;
        float elapsedTime = 0.0f;
       

        void GenerateParticle();

      public:
        ParticleEmitter() = default;
        //ParticleEmitter(float rate);
        
        void InitParticle(Ref<Particle> p);
        void Update();
        void Render();

       

        template <class Archive> void serialize(Archive& archive)
        {
            archive(activeParticles, rateOverTime, elapsedTime);
        }
    };

} // namespace Coffee