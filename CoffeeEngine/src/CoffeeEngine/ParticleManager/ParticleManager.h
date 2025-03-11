#pragma once
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Math/BoundingBox.h"
#include <CoffeeEngine/ImGui/ImGuiExtras.h>
#include <CoffeeEngine/Scene/PrimitiveMesh.h>

namespace Coffee
{
    class ParticleEmitter;

    struct Particle
    {
        glm::mat4 transformMatrix;
        glm::vec3 direction;
        glm::vec4 color;
        float size;
        float lifetime;
        float startLifetime;
        float startSpeed;
        glm::vec3 startSize;
        glm::vec3 startRotation;

        Particle();
        glm::mat4 GetWorldTransform();
        void SetPosition(glm::vec3 position);
        void SetRotation(glm::vec3 rotation);
        void SetSize(glm::vec3 size);
        glm::vec3 GetPosition();
        glm::vec3 GetRotation();
        glm::vec3 GetSize();
    };

    class ParticleEmitter
    {
      public:
        glm::mat4 transformComponentMatrix;
        

        bool useDirectionRandom = false;
        glm::vec3 direction = {0.0f, 1.0f, 0.0f};
        glm::vec3 directionRandom = {0.0f, 1.0f, 0.0f};

        bool useColorRandom = false;
        glm::vec4 colorNormal = {1.0f, 1.0f, 1.0f, 1.0f};
        glm::vec4 colorRandom = {1.0f, 1.0f, 1.0f, 1.0f};

        int amount = 1000;
        bool looping = true;

        bool useRandomLifeTime = false;
        float startLifeTimeMin = 5.0f;
        float startLifeTimeMax = 5.0f;
        float startLifeTime = 5.0f;

        bool useRandomSpeed = false;
        float startSpeedMin = 5.0f;
        float startSpeedMax = 5.0f;
        float startSpeed = 5.0f;

        bool useRandomSize = false;
        bool useSplitAxesSize = false;
        glm::vec3 startSizeMin = glm::vec3(1, 1, 1);
        glm::vec3 startSizeMax = glm::vec3(1, 1, 1);
        glm::vec3 startSize = glm::vec3(1, 1, 1);

        bool useRandomRotation = false;
        glm::vec3 startRotationMin = glm::vec3(0, 0, 0);
        glm::vec3 startRotationMax = glm::vec3(0, 0, 0);
        glm::vec3 startRotation = glm::vec3(0, 0, 0);

        enum class SimulationSpace
        {
            Local = 0,
            World = 1,
            Custom = 2
        };

        SimulationSpace simulationSpace = SimulationSpace::World;

        bool useEmission = true;
        float rateOverTime = 1.0f;

        enum class ShapeType
        {
            Sphere,
            Cone,
            Box
        };

        ShapeType shape = ShapeType::Box;
        glm::vec3 minSpread = {0.0f, 0.0f, 0.0f};
        glm::vec3 maxSpread = {0.0f, 0.0f, 0.0f};
        bool useShape = true;
        float shapeAngle = 45.0f;
        float shapeRadius = 1.0f;
        float shapeRadiusThickness = 0.1f;

        bool useVelocityOverLifetime = false;
        bool velocityOverLifeTimeSeparateAxes = false;
        std::vector<CurvePoint> speedOverLifeTimeX = {{0.0f, 1.0f}, {1.0f, 0.5f}};
        std::vector<CurvePoint> speedOverLifeTimeY = {{0.0f, 1.0f}, {1.0f, 0.5f}};
        std::vector<CurvePoint> speedOverLifeTimeZ = {{0.0f, 1.0f}, {1.0f, 0.5f}};
        std::vector<CurvePoint> speedOverLifeTimeGeneral = {{0.0f, 1.0f}, {1.0f, 0.5f}};

        bool useSizeOverLifetime = false;
        bool sizeOverLifeTimeSeparateAxes = false;
        std::vector<CurvePoint> sizeOverLifetimeX = {{0.0f, 1.0f}, {1.0f, 0.5f}};
        std::vector<CurvePoint> sizeOverLifetimeY = {{0.0f, 1.0f}, {1.0f, 0.5f}};
        std::vector<CurvePoint> sizeOverLifetimeZ = {{0.0f, 1.0f}, {1.0f, 0.5f}};
        std::vector<CurvePoint> sizeOverLifetimeGeneral = {{0.0f, 1.0f}, {1.0f, 0.5f}};

        bool useRotationOverLifetime = false;
        std::vector<CurvePoint> rotationOverLifetimeX = {{0.0f, 1.0f}, {1.0f, 0.5f}};
        std::vector<CurvePoint> rotationOverLifetimeY = {{0.0f, 1.0f}, {1.0f, 0.5f}};
        std::vector<CurvePoint> rotationOverLifetimeZ = {{0.0f, 1.0f}, {1.0f, 0.5f}};

        bool useColorOverLifetime = false;
        glm::vec4 overLifetimecolor;
        std::vector<GradientPoint> colorOverLifetime_gradientPoints = {{0.0f, ImVec4(1, 1, 1, 1)},
                                                                       {0.3f, ImVec4(1, 1, 1, 1)}};

        bool useRenderer = true;
        int renderMode = 0;
        glm::mat4 cameraViewMatrix;

        static Ref<Mesh> particleMesh;
        Ref<Material> particleMaterial;
        int renderAlignment = 0;

        std::vector<Ref<Particle>> activeParticles;

      private:
        float accumulatedParticles = 0.0f;
        float elapsedTime = 0.0f;

        void GenerateParticle();

      public:
        ParticleEmitter();
        void InitParticle(Ref<Particle> particle);
        void Update(float deltaTime);
        void UpdateParticle(Ref<Particle> particle, float deltaTime);

        glm::mat4 CalculateBillboardTransform(const glm::mat4& particleTransform, const glm::mat4& viewMatrix);


        template <class Archive> void serialize(Archive& archive)
        {
            // archive(rateOverTime, elapsedTime);
        }

    };
} // namespace Coffee