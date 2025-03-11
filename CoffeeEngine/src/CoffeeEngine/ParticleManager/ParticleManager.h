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
        //glm::vec3 position;
        glm::mat4 transformMatrix;


       

        glm::vec3 direction;
        glm::vec4 color;
        float size;
        float lifetime;



        

        Particle();
        // void Init();
        //void Update(float dt);

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


        // Direction
        bool useDirectionRandom = false;
        glm::vec3 direction = {0.0f, 1.0f, 0.0f};
        glm::vec3 directionRandom = {0.0f, 1.0f, 0.0f};

        //Color
        bool useColorRandom = false;
        glm::vec4 colourNormal = {1.0f, 1.0f, 1.0f, 1.0f};
        glm::vec4 colourRandom = {1.0f, 1.0f, 1.0f, 1.0f};
        

        int amount = 1000;
       

        // Looping
        bool looping = true;

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

        SimulationSpace simulationSpace = SimulationSpace::World; // Default value

        //Emission
        bool useEmission = true;
        float rateOverTime = 1.0f;



        // Shape
        enum class ShapeType
        {
            Sphere,
            Cone,
            Box
        };
        ShapeType shape = ShapeType::Sphere; 
        glm::vec3 minSpread = {0.0f, 0.0f, 0.0f};
        glm::vec3 maxSpread = {0.0f, 0.0f, 0.0f};
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
        bool useColorOverLifetime = false;
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
        bool useRotationOverLifetime = false;
        bool rotationSeparateAxes = false;
        float rotationOverLifetimeX;
        float rotationOverLifetimeY;
        float rotationOverLifetimeZ;
        glm::vec3 rotationOverLifetime;
        float rotationOverLifetimeAngularVelocity;

        // Renderer
        bool useRenderer = true;
        int renderMode = 0;
        static Ref<Mesh> particleMesh;
        int renderAlignment = 0;

         std::vector<Ref<Particle>> activeParticles;



      private:
       
    
        float accumulatedParticles = 0.0f;
        float elapsedTime = 0.0f;

        void GenerateParticle();

      public:
        ParticleEmitter();
        // ParticleEmitter(float rate);

        void InitParticle(Ref<Particle> p);
        void Update(float dt);
        void UpdateParticle(Ref<Particle> p, float dt);



        template <class Archive> void serialize(Archive& archive)
        {
            //archive(rateOverTime, elapsedTime);
        }
    };

} // namespace Coffee