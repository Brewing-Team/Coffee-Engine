#include "CoffeeEngine/Physics/PhysicsWorld.h"
#include "CoffeeEngine/Physics/CollisionSystem.h"
#include "CoffeeEngine/Renderer/Renderer2D.h"

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
        for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
            btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* body = btRigidBody::upcast(obj);
            if (body) {
                dynamicsWorld->removeRigidBody(body);
            }
        }

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

    void PhysicsWorld::stepSimulation(const float dt) const {
        dynamicsWorld->stepSimulation(dt);
        CollisionSystem::checkCollisions(*this);
    }

    void PhysicsWorld::setGravity(const float gravity) const {
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
        if (!dynamicsWorld) return;
        const int numCollisionObjects = dynamicsWorld->getNumCollisionObjects();
        for (int i = 0; i < numCollisionObjects; i++) {
            constexpr float margin = 0.05f;
            const btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
            
            if (!obj) continue;
            
            const btCollisionShape* shape = obj->getCollisionShape();
            
            if (!shape) continue;
            
            const btTransform& transform = obj->getWorldTransform();

            btVector3 origin = transform.getOrigin();
            btQuaternion rotation = transform.getRotation();
            glm::vec3 position(origin.x(), origin.y(), origin.z());
            glm::quat orientation(rotation.w(), rotation.x(), rotation.y(), rotation.z());

            switch (shape->getShapeType()) {
                case BOX_SHAPE_PROXYTYPE: {
                    const btBoxShape* boxShape = static_cast<const btBoxShape*>(shape);
                    if (!boxShape) continue;
    
                    btVector3 halfExtents = boxShape->getHalfExtentsWithMargin();
                    glm::vec3 size((halfExtents.x() + margin) * 2.0f, (halfExtents.y() + margin) * 2.0f, (halfExtents.z() + margin) * 2.0f);
                    Renderer2D::DrawBox(position, orientation, size, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
                    break;
                }
                case SPHERE_SHAPE_PROXYTYPE: {
                    const btSphereShape* sphereShape = static_cast<const btSphereShape*>(shape);
                    if (!sphereShape) continue;
                    
                    const float radius = sphereShape->getRadius() + margin;
                    Renderer2D::DrawSphere(position, radius, orientation, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
                    break;
                }
                case CAPSULE_SHAPE_PROXYTYPE: {
                    const btCapsuleShape* capsuleShape = static_cast<const btCapsuleShape*>(shape);
                    if (!capsuleShape) continue;
                    
                    const float radius = capsuleShape->getRadius() + margin;
                    const float cylinderHeight = capsuleShape->getHalfHeight() * 2.0f + margin;
                    
                    Renderer2D::DrawCapsule(position, orientation, radius, cylinderHeight, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
                    break;
                }
                default:
                    continue;
            }
        }
    }

} // namespace Coffee