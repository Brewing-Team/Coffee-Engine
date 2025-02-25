#include "CoffeeEngine/Physics/CollisionSystem.h"
#include "CoffeeEngine/Physics/PhysicsWorld.h"

namespace Coffee {

    void CollisionSystem::checkCollisions(const PhysicsWorld& world) {
        int numManifolds = world.getDynamicsWorld()->getDispatcher()->getNumManifolds();
        for (int i = 0; i < numManifolds; i++) {
            btPersistentManifold* contactManifold = world.getDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* objA = contactManifold->getBody0();
            const btCollisionObject* objB = contactManifold->getBody1();

            if (contactManifold->getNumContacts() > 0) {
                // Handle collision between objA and objB
                COFFEE_CORE_INFO("Collision System: Collision detected!");
            }
        }
    }

}