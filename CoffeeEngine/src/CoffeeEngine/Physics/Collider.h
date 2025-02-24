//
// Created by mdoradom on 24/02/25.
//

#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

namespace Coffee {

    class Collider {
    protected:
        btCollisionShape* shape;

    public:
        virtual ~Collider() {
            // delete shape;
        }

        btCollisionShape* getShape() const {
            return shape;
        }
    };

    class BoxCollider : public Collider {
    public:
        BoxCollider(const glm::vec3& size) {
            shape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
        }
    };

    class SphereCollider : public Collider {
    public:
        SphereCollider(float radius) {
            shape = new btSphereShape(radius);
        }
    };

    class CapsuleCollider : public Collider {
    public:
        CapsuleCollider(float radius, float height) {
            shape = new btCapsuleShape(radius, height);
        }
    };

}
