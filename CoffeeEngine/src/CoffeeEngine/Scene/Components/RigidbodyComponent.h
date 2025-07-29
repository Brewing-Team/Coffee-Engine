#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Physics/CollisionCallback.h"
#include "CoffeeEngine/Physics/RigidBody.h"
#include <cereal/cereal.hpp>

namespace Coffee {
  
    class Collider;
}

namespace Coffee
{
    struct RigidbodyComponent
    {
      public:
        Ref<RigidBody> rb;
        CollisionCallback callback;

        RigidbodyComponent() = default;
        RigidbodyComponent(const RigidbodyComponent&) = default;
        RigidbodyComponent(const RigidBody::Properties& props, Ref<Collider> collider)
        {
            rb = RigidBody::Create(props, collider);
        }

        ~RigidbodyComponent() { rb.reset(); }

      private:
        friend class cereal::access;

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;
        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::RigidbodyComponent, 0);
