#include "RigidbodyComponent.h"
#include "CoffeeEngine/Physics/Collider.h"
#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee 
{
    template <class Archive> 
    void RigidbodyComponent::save(Archive& archive, std::uint32_t const version) const
    {
        if (rb)
        {
            archive(cereal::make_nvp("RigidBody", true), cereal::make_nvp("RigidBodyData", rb));
        }
        else
        {
            archive(cereal::make_nvp("RigidBody", false));
        }
    }

    template <class Archive> 
    void RigidbodyComponent::load(Archive& archive, std::uint32_t const version)
    {
        bool hasRigidBody;
        archive(cereal::make_nvp("RigidBody", hasRigidBody));

        if (hasRigidBody)
        {
            archive(cereal::make_nvp("RigidBodyData", rb));
        }
    }

    // Explicit template instantiations for common cereal archives
    template void RigidbodyComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const) const;
    template void RigidbodyComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void RigidbodyComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const) const;
    template void RigidbodyComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
}
