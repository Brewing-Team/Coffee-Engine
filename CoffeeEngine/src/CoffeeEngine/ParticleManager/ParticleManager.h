#pragma once
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Math/BoundingBox.h"
#include <CoffeeEngine/ImGui/ImGuiExtras.h>

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
        // void Init();
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

        // Looping
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

        // Start Rotation
        bool useRandomRotation = false;
        float startRotationMin = 5.0f;
        float startRotationMax = 5.0f;
        float startRotation = 5.0f;

        // SimulationSpace
        enum class SimulationSpace
        {
            Local = 0,
            World = 1,
            Custom = 2
        };

        SimulationSpace simulationSpace = SimulationSpace::Local; // Default value

        // Shape
        enum class ShapeType
        {
            Sphere,
            Cone,
            Box
        };
        ShapeType shape = ShapeType::Sphere; 
        bool useShape = false;
        float shapeangle = 45.0f;                           
        float shaperadius = 1.0f;                       
        float shaperadiusThickness = 0.1f;                   

        // VelocityOverLifetime

        enum class Space
        {
            Local = 0,
            World = 1,
        };

        Space Space = Space::Local; // Default value

        bool useVelocityOverLifetime = false;
        glm::vec3 linearX = {0.0f, 0.0f, 0.0f};
        float space;
        glm::vec3 orbitalX = {0.0f, 0.0f, 0.0f};
        glm::vec3 offsetX;
        float radial;
        float speedModifier;

        // ColorOverLifetime
        bool useColorOverLifetime;
        glm::vec4 overLifetimecolor;

        std::vector<GradientPoint> colorOverLifetime_gradientPoints = {
            {0.0f, ImVec4(1, 1, 1, 1)},
            {0.3f, ImVec4(1, 1, 1, 1)}
        };

        // Size over Lifetime
        bool useSizeOverLifetime = false;
        bool separateAxes = false;
        std::vector<CurvePoint> sizeOverLifetimeX = {{0.0f, 1.0f}, {1.0f, 0.5f}};
        std::vector<CurvePoint> sizeOverLifetimeY = {{0.0f, 1.0f}, {1.0f, 0.5f}};
        std::vector<CurvePoint> sizeOverLifetimeZ = {{0.0f, 1.0f}, {1.0f, 0.5f}};
        std::vector<CurvePoint> sizeOverLifetimeGeneral = {{0.0f, 1.0f}, {1.0f, 0.5f}};



        // Rotation over Lifetime
        bool useRotationOverLifetime;
        bool rotationSeparateAxes = false;
        float rotationOverLifetimeX;
        float rotationOverLifetimeY;
        float rotationOverLifetimeZ;
        glm::vec3 rotationOverLifetime;
        float rotationOverLifetimeAngularVelocity;

        // Renderer
        bool useRenderer = false;
        int renderMode = 0;
        char material[256];
        int renderAlignment = 0;

      private:
        std::vector<Ref<Particle>> activeParticles;
        float rateOverTime = 1.0f;
        float elapsedTime = 0.0f;

        void GenerateParticle();

      public:
        ParticleEmitter() = default;
        // ParticleEmitter(float rate);

        void InitParticle(Ref<Particle> p);
        void Update();
        void Render();

        template <class Archive> void serialize(Archive& archive)
        {
            archive(activeParticles, rateOverTime, elapsedTime);
        }
    };

} // namespace Coffee