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
    };

}