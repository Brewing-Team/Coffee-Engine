#include "CoffeeEngine/Physics/PhysicsWorld.h"
#include "CoffeeEngine/Physics/CollisionSystem.h"

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

    btDiscreteDynamicsWorld* PhysicsWorld::getDynamicsWorld() const {
        return dynamicsWorld;
    }

}