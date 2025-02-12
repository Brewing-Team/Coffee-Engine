#pragma once

#include "CoffeeEngine/Physics/Collider.h"
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Coffee {

    struct RigidBodyConfig
    {
        enum class MotionType {
            Static,
            Dynamic,
            Kinematic
        };

        MotionType motionType = MotionType::Dynamic;
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 linearVelocity = glm::vec3(0.0f);
        glm::vec3 angularVelocity = glm::vec3(0.0f);
        Collider collider = Collider();

        float gravityFactor = 1.0f;

        float linearDamping = 0.0f;
        float angularDamping = 0.0f;

        bool FreezePositionX = false;
        bool FreezePositionY = false;
        bool FreezePositionZ = false;
        bool FreezeRotationX = false;
        bool FreezeRotationY = false;
        bool FreezeRotationZ = false;
    };

    class RigidBody
    {
    public:
        RigidBody(const RigidBodyConfig& config);
        ~RigidBody();

        void SetPosition(const glm::vec3& position);
        glm::vec3 GetPosition();
        void SetRotation(const glm::quat& rotation);
        void SetLinearVelocity(const glm::vec3& linearVelocity);
        void SetAngularVelocity(const glm::vec3& angularVelocity);

        void SetGravityFactor(float gravityFactor);
        void SetFriction(float friction);

        void SetFreezePositionX(bool freeze);
        void SetFreezePositionY(bool freeze);
        void SetFreezePositionZ(bool freeze);
        void SetFreezeRotationX(bool freeze);
        void SetFreezeRotationY(bool freeze);
        void SetFreezeRotationZ(bool freeze);

        void ApplyForce(const glm::vec3& force);
        void ApplyForceAtPosition(const glm::vec3& force, const glm::vec3& position);
        void ApplyTorque(const glm::vec3& torque);

        void SetCollisionLayer(uint32_t layer);
        void SetCollisionMask(uint32_t mask);

        void SetCollisionFilter(uint32_t layer, uint32_t mask);

        void SetCollisionGroup(uint32_t group);
        void SetCollisionGroupMask(uint32_t mask);

        void SetCollisionGroupFilter(uint32_t group, uint32_t mask);

        void SetCollisionLayerAndMask(uint32_t layer, uint32_t mask);
        void SetCollisionGroupAndMask(uint32_t group, uint32_t mask);

        JPH::BodyID GetBodyID() const { return m_BodyID; }


    private:
        JPH::BodyCreationSettings m_BodySettings;
        JPH::Body* m_Body;
        JPH::BodyID m_BodyID;

        uint32_t m_CollisionLayer;
        uint32_t m_CollisionMask;
        uint32_t m_CollisionGroup;
        uint32_t m_CollisionGroupMask;

        friend class PhysicsEngine;
    };

}

