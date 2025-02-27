//
// Created by mdoradom on 24/02/25.
//

#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

namespace Coffee {

    enum class RigidBodyType {
        Static = 0,
        Dynamic,
        Kinematic
    };

    struct CollisionShapeConfig {
        enum class Type { Box, Sphere, Capsule };
        Type type = Type::Box;
        glm::vec3 size = {1.0f, 1.0f, 1.0f};
        bool isTrigger = false;
        float mass = 1.0f;
    };

    struct RigidBodyConfig {
        CollisionShapeConfig shapeConfig;
        RigidBodyType type = RigidBodyType::Dynamic;
        bool UseGravity = true;
        glm::vec3 Velocity = {0.0f, 0.0f, 0.0f};
        bool FreezeY = false;
    };

    struct Rigidbody {
        RigidBodyConfig cfg;
        btRigidBody* Body = nullptr;
        btCollisionShape* Shape = nullptr;
        btMotionState* MotionState = nullptr;

        Rigidbody() = default;
        ~Rigidbody() {
            if (Body) {
                delete Body->getMotionState();
                delete Body;
            }
            if (Shape) delete Shape;
            // if (MotionState) delete MotionState;
        }

        void SetContactProcessingThreshold(float threshold) {
            Body->setContactProcessingThreshold(threshold);
        }

        #pragma region Getters
        glm::vec3 GetPosition() const
        {
            btVector3 btPos = Body->getWorldTransform().getOrigin();
            return glm::vec3(btPos.x(), btPos.y(), btPos.z());
        }

        glm::vec3 GetRotation() const
        {
            btQuaternion btRot = Body->getWorldTransform().getRotation();
            return glm::vec3(btRot.x(), btRot.y(), btRot.z());
        }

        // This Functions returns all the forces applied to the rigidbody in all directions
        glm::vec3 GetDirection() const
        {
            btVector3 btDir = Body->getLinearVelocity();
            return glm::vec3(btDir.x(), btDir.y(), btDir.z());
        }

        bool isTrigger()
        {
            return cfg.shapeConfig.isTrigger;
        }
        #pragma endregion

        #pragma region Setters
        void SetPosition(const glm::vec3& position);
        void SetRotation(const glm::vec3& rotation);
        void setTrigger(bool setTrigger);
        #pragma endregion

        // Functions
        // Apply a force to the rigidbody: Continuous force
        void applyForce(const glm::vec3& force) const {
            Body->activate(true); // Activate the rigidbody in case it's sleeping
            Body->applyCentralForce(btVector3(force.x, force.y, force.z));
        }

        // Apply an impulse to the rigidbody: Instant force
        void applyImpulse(const glm::vec3& impulse) const
        {
            Body->activate(true); // Activate the rigidbody in case it's sleeping
            Body->applyCentralImpulse(btVector3(impulse.x, impulse.y, impulse.z));
        }

    };

}