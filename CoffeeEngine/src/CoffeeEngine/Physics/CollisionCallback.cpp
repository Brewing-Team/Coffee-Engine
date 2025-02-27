#include "CollisionCallback.h"
#include "CoffeeEngine/Scene/Entity.h"
#include <btBulletDynamicsCommon.h>

namespace Coffee {

    void CollisionCallback::OnCollisionEnter(CollisionFunction fn) {
        m_OnCollisionEnter = fn;
    }

    void CollisionCallback::OnCollisionStay(CollisionFunction fn) {
        m_OnCollisionStay = fn;
    }

    void CollisionCallback::OnCollisionExit(CollisionFunction fn) {
        m_OnCollisionExit = fn;
    }

    const CollisionCallback::CollisionFunction& CollisionCallback::GetOnCollisionEnter() const {
        return m_OnCollisionEnter;
    }

    const CollisionCallback::CollisionFunction& CollisionCallback::GetOnCollisionStay() const {
        return m_OnCollisionStay;
    }

    const CollisionCallback::CollisionFunction& CollisionCallback::GetOnCollisionExit() const {
        return m_OnCollisionExit;
    }
}