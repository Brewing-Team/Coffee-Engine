#pragma once

#define BT_NO_SIMD_OPERATOR_OVERLOADS

#include <unordered_set>
#include <btBulletDynamicsCommon.h>

namespace Coffee {
    
    class Scene;
    class PhysicsWorld;
}

namespace Coffee {

    // Helper struct to hash collision pair
    struct PairHash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& p) const {
            return std::hash<T1>{}(p.first) ^ std::hash<T2>{}(p.second);
        }
    };

    class CollisionSystem {
    public:
        void Initialize(Scene* scene);
        void checkCollisions(const PhysicsWorld& world);
        void Shutdown();

      private:
        Scene* m_Scene;
        std::unordered_set<std::pair<btCollisionObject*, btCollisionObject*>, PairHash> m_ActiveCollisions;
    };

}