#include "CoffeeEngine/Physics/PhysicsWorld.h"
#include "CoffeeEngine/Physics/CollisionSystem.h"
#include "CoffeeEngine/Renderer/DebugRenderer.h"

#include <glm/fwd.hpp>

namespace Coffee {

    PhysicsWorld::PhysicsWorld() {
        collisionConfig = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfig);
        broadphase = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver();
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
        dynamicsWorld->setGravity(btVector3(0, GRAVITY, 0));
    }

    PhysicsWorld::~PhysicsWorld() {
        delete dynamicsWorld;
        delete solver;
        delete broadphase;
        delete dispatcher;
        delete collisionConfig;
    }

    void PhysicsWorld::addRigidBody(btRigidBody* body) const {
        dynamicsWorld->addRigidBody(body);
    }

    void PhysicsWorld::removeRigidBody(btRigidBody* body) const {
        dynamicsWorld->removeRigidBody(body);
    }

    void PhysicsWorld::stepSimulation(float dt) const {
        dynamicsWorld->stepSimulation(dt);
        CollisionSystem::checkCollisions(*this);
    }

    void PhysicsWorld::setGravity(float gravity) const {
        dynamicsWorld->setGravity(btVector3(0, gravity, 0));
    }

    void PhysicsWorld::setGravity(const btVector3& gravity) const {
        dynamicsWorld->setGravity(gravity);
    }

    btDiscreteDynamicsWorld* PhysicsWorld::getDynamicsWorld() const
    {
        return dynamicsWorld;
    }

    void PhysicsWorld::drawCollisionShapes() const {
        const float margin = 0.05f;

        int numCollisionObjects = dynamicsWorld->getNumCollisionObjects();
        for (int i = 0; i < numCollisionObjects; i++) {
            const btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
            const btCollisionShape* shape = obj->getCollisionShape();
            const btTransform& transform = obj->getWorldTransform();

            btVector3 origin = transform.getOrigin();
            btQuaternion rotation = transform.getRotation();
            glm::vec3 position(origin.x(), origin.y(), origin.z());
            glm::quat orientation(rotation.w(), rotation.x(), rotation.y(), rotation.z());

            if (shape->getShapeType() == BOX_SHAPE_PROXYTYPE) {
                const btBoxShape* boxShape = static_cast<const btBoxShape*>(shape);
                btVector3 halfExtents = boxShape->getHalfExtentsWithMargin();
                glm::vec3 size((halfExtents.x() + margin) * 2.0f, (halfExtents.y() + margin) * 2.0f, (halfExtents.z() + margin) * 2.0f);
                DebugRenderer::DrawBox(position, orientation, size, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            } else if (shape->getShapeType() == SPHERE_SHAPE_PROXYTYPE) {
                const btSphereShape* sphereShape = static_cast<const btSphereShape*>(shape);
                float radius = sphereShape->getRadius() + margin;
                DebugRenderer::DrawSphere(position, radius, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            } else if (shape->getShapeType() == CAPSULE_SHAPE_PROXYTYPE) {
                const btCapsuleShape* capsuleShape = static_cast<const btCapsuleShape*>(shape);
                float radius = capsuleShape->getRadius() + margin;
                float height = capsuleShape->getHalfHeight() * 2.0f + margin;
                DebugRenderer::DrawBox(position, orientation, glm::vec3(radius * 2.0f, height, radius * 2.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
            } else {
                // Handle other shape types if needed
            }
        }
    }

} // namespace Coffee