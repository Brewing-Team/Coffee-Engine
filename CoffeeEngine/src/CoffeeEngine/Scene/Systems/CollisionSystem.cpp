#include "CollisionSystem.h"
#include "CoffeeEngine/Physics/Runtime/PhysicsWorld.h"
#include "CoffeeEngine/Scene/Entity.h"
#include <BulletCollision/NarrowPhaseCollision/btPersistentManifold.h>
#include <tracy/Tracy.hpp>

namespace Coffee {

    void CollisionSystem::Initialize(Scene* scene) {
        m_Scene = scene;
    }

    void CollisionSystem::checkCollisions(const PhysicsWorld& world) {

        ZoneScoped;

        std::unordered_set<std::pair<btCollisionObject*, btCollisionObject*>, PairHash> currentCollisions;

        int numManifolds = world.getDynamicsWorld()->getDispatcher()->getNumManifolds();
        for (int i = 0; i < numManifolds; i++) {
            ::btPersistentManifold* contactManifold = world.getDynamicsWorld()->getDispatcher()->getManifoldByIndexInternal(i);
            auto objA = const_cast<btCollisionObject*>(contactManifold->getBody0());
            auto objB = const_cast<btCollisionObject*>(contactManifold->getBody1());

            // Get entities from collision objects
            Entity entityA(static_cast<entt::entity>(reinterpret_cast<size_t>(objA->getUserPointer())), m_Scene);
            Entity entityB(static_cast<entt::entity>(reinterpret_cast<size_t>(objB->getUserPointer())), m_Scene);

            if (contactManifold->getNumContacts() > 0) {
                auto pair = std::make_pair(objA, objB);
                currentCollisions.insert(pair);

                // CollisionInfo info = { entityA, entityB, contactManifold };
                CollisionInfo info{entityA, entityB, contactManifold};

                // Handle collision enter
                if (m_ActiveCollisions.find(pair) == m_ActiveCollisions.end()) {
                    if (entityA.HasComponent<RigidbodyComponent>() && entityB.HasComponent<RigidbodyComponent>()) {
                        auto& rbA = entityA.GetComponent<RigidbodyComponent>();
                        auto& rbB = entityB.GetComponent<RigidbodyComponent>();

                        if (rbA.callback.GetOnCollisionEnter())
                            rbA.callback.GetOnCollisionEnter()(info);
                        if (rbB.callback.GetOnCollisionEnter())
                            rbB.callback.GetOnCollisionEnter()(info);
                    }
                }
                // Handle collision stay
                else {
                    if (entityA.HasComponent<RigidbodyComponent>() && entityB.HasComponent<RigidbodyComponent>())
                    {
                        auto& rbA = entityA.GetComponent<RigidbodyComponent>();
                        auto& rbB = entityB.GetComponent<RigidbodyComponent>();

                        if (rbA.callback.GetOnCollisionStay())
                            rbA.callback.GetOnCollisionStay()(info);
                        if (rbB.callback.GetOnCollisionStay())
                            rbB.callback.GetOnCollisionStay()(info);
                    }
                }
            }
        }

        // Handle collision exit
        for (const auto& pair : m_ActiveCollisions) {
            if (currentCollisions.find(pair) == currentCollisions.end()) {
                Entity entityA(static_cast<entt::entity>(reinterpret_cast<size_t>(pair.first->getUserPointer())), m_Scene);
                Entity entityB(static_cast<entt::entity>(reinterpret_cast<size_t>(pair.second->getUserPointer())), m_Scene);

                CollisionInfo info = { entityA, entityB, nullptr };

                if (entityA.HasComponent<RigidbodyComponent>() && entityB.HasComponent<RigidbodyComponent>())
                {
                    auto& rbA = entityA.GetComponent<RigidbodyComponent>();
                    auto& rbB = entityB.GetComponent<RigidbodyComponent>();

                    if (rbA.callback.GetOnCollisionExit())
                        rbA.callback.GetOnCollisionExit()(info);
                    if (rbB.callback.GetOnCollisionExit())
                        rbB.callback.GetOnCollisionExit()(info);
                }
            }
        }

        m_ActiveCollisions = currentCollisions;
    }

    void CollisionSystem::Shutdown() {
        m_ActiveCollisions.clear();
        m_Scene = nullptr;
    }

}