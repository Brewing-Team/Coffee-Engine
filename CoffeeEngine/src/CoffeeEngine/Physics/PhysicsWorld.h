//
// Created by mdoradom on 24/02/25.
//

#include <btBulletDynamicsCommon.h>
#include <vector>

#pragma once

namespace Coffee {

    const float GRAVITY = -9.81f;

    class PhysicsWorld {

    public:
        PhysicsWorld() {
            collisionConfig = new btDefaultCollisionConfiguration();
            dispatcher = new btCollisionDispatcher(collisionConfig);
            broadphase = new btDbvtBroadphase();
            solver = new btSequentialImpulseConstraintSolver();
            dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
            dynamicsWorld->setGravity(btVector3(0, GRAVITY, 0));
        }

        ~PhysicsWorld() {
            delete dynamicsWorld;
            delete solver;
            delete broadphase;
            delete dispatcher;
            delete collisionConfig;
        }

        void addRigidBody(btRigidBody* body) {
            dynamicsWorld->addRigidBody(body);
        }

        void removeRigidBody(btRigidBody* body) {
            dynamicsWorld->removeRigidBody(body);
        }

        void stepSimulation(float dt) {
            dynamicsWorld->stepSimulation(dt);
        }

        // ---- Gravity ----
        void setGravity(float gravity) {
            dynamicsWorld->setGravity(btVector3(0, gravity, 0));
        }

        void setGravity(const btVector3& gravity) { dynamicsWorld->setGravity(gravity); }
        // -----------------

        btDiscreteDynamicsWorld* getDynamicsWorld() const { return dynamicsWorld; }

    private:
        btDefaultCollisionConfiguration* collisionConfig;    ///< Configuration for the collision
        btCollisionDispatcher* dispatcher;                   ///< Dispatcher for the collision
        btBroadphaseInterface* broadphase;                   ///< Broadphase for the collision
        btSequentialImpulseConstraintSolver* solver;         ///< Solver for the collision
        btDiscreteDynamicsWorld* dynamicsWorld;              ///< World for the collision
    };

}