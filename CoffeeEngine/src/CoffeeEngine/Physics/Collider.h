//
// Created by mdoradom on 24/02/25.
//

#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

namespace Coffee {

    class Collider {
    protected:
        btCollisionShape* shape = nullptr;

    public:
        virtual ~Collider() {
            delete shape;
        }

        btCollisionShape* getShape() const {
            return shape;
        }
    };

    class BoxCollider final : public Collider {
    public:
        BoxCollider(const glm::vec3& size) {
            shape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
        }
    };

    class SphereCollider final : public Collider {
    public:
        SphereCollider(const float radius) {
            shape = new btSphereShape(radius);
        }
    };

    class CapsuleCollider final : public Collider {
    public:
        CapsuleCollider(const float radius, const float height) {
            shape = new btCapsuleShape(radius, height);
        }
    };

}
