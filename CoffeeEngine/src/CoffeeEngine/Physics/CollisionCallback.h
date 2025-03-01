#pragma once
#include <functional>
#include <BulletCollision/NarrowPhaseCollision/btPersistentManifold.h>

namespace Coffee {
    class Entity;
    struct btPersistentManifold;

    struct CollisionInfo {
        Entity& entityA;
        Entity& entityB;
        ::btPersistentManifold* manifold;
    };

    class CollisionCallback {
    public:
        using CollisionFunction = std::function<void(CollisionInfo&)>;

        void OnCollisionEnter(const CollisionFunction& fn);
        void OnCollisionStay(const CollisionFunction& fn);
        void OnCollisionExit(const CollisionFunction& fn);

        const CollisionFunction& GetOnCollisionEnter() const;
        const CollisionFunction& GetOnCollisionStay() const;
        const CollisionFunction& GetOnCollisionExit() const;

    private:
        CollisionFunction m_OnCollisionEnter;
        CollisionFunction m_OnCollisionStay;
        CollisionFunction m_OnCollisionExit;
    };
}