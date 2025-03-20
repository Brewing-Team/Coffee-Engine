#pragma once

#define BT_NO_SIMD_OPERATOR_OVERLOADS

#include "CoffeeEngine/Scene/Entity.h"

#include <btBulletDynamicsCommon.h>
#include <vector>

namespace Coffee {

    constexpr float GRAVITY = -9.81f;

    struct RaycastHit {
        bool hasHit = false;
        Entity hitEntity;
        glm::vec3 hitPoint = {0.0f, 0.0f, 0.0f};
        glm::vec3 hitNormal = {0.0f, 0.0f, 0.0f};
        float hitFraction = 1.0f;  // Fraction of the ray where the hit occurred
    };

    class PhysicsWorld {

    public:
        PhysicsWorld();
        ~PhysicsWorld();

        void addRigidBody(btRigidBody* body) const;
        void removeRigidBody(btRigidBody* body) const;
        void stepSimulation(float dt) const;

        void setGravity(float gravity) const;
        void setGravity(const btVector3& gravity) const;

        btDiscreteDynamicsWorld* getDynamicsWorld() const;

        void drawCollisionShapes() const;

        // Single raycast (returns closest hit)
        RaycastHit Raycast(const glm::vec3& origin, const glm::vec3& direction, float maxDistance = 1000.0f) const;
        
        // Multiple raycast (returns all hits sorted by distance)
        std::vector<RaycastHit> RaycastAll(const glm::vec3& origin, const glm::vec3& direction, float maxDistance = 1000.0f) const;
        
        // Quick test for any collision along ray path
        bool RaycastAny(const glm::vec3& origin, const glm::vec3& direction, float maxDistance = 1000.0f) const;

    private:
        btDefaultCollisionConfiguration* collisionConfig;
        btCollisionDispatcher* dispatcher;
        btBroadphaseInterface* broadphase;
        btSequentialImpulseConstraintSolver* solver;
        btDiscreteDynamicsWorld* dynamicsWorld;
    };

}