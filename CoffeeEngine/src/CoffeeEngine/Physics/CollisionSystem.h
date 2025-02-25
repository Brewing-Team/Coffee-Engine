//
// Created by mdoradom on 24/02/25.
//

#pragma once

#include "CoffeeEngine/Core/Log.h"
#include "CoffeeEngine/Physics/PhysicsWorld.h"
#include <btBulletDynamicsCommon.h>

namespace Coffee {

    class PhysicsWorld;

    class CollisionSystem {
    public:
        static void checkCollisions(const PhysicsWorld& world) ;
    };

}