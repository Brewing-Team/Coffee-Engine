#include "Skeleton.h"

#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee {

    // Joint implementation
    template<class Archive>
    void Joint::serialize(Archive& archive, std::uint32_t const version)
    {
        archive(name, parentIndex);

        archive(localTransform.translation.x,
               localTransform.translation.y,
               localTransform.translation.z);

        archive(localTransform.rotation.x,
               localTransform.rotation.y,
               localTransform.rotation.z,
               localTransform.rotation.w);

        archive(localTransform.scale.x,
               localTransform.scale.y,
               localTransform.scale.z);

        archive(invBindPose);
    }

    void Skeleton::SetSkeleton(ozz::unique_ptr<ozz::animation::Skeleton> skeleton)
    {
        m_Skeleton = std::move(skeleton);
        m_NumJoints = m_Skeleton->num_joints();
        m_JointMatrices.resize(m_NumJoints);
    }

    void Skeleton::SetJoints(const std::vector<Joint>& joints)
    {
        m_Joints = joints;
    }

    void Skeleton::Save(ozz::io::OArchive& archive) const
    {
        m_Skeleton->Save(archive);
    }

    void Skeleton::Load(ozz::io::IArchive& archive, std::vector<Joint>& joints)
    {
        m_Skeleton->Load(archive, 2);
        m_NumJoints = m_Skeleton->num_joints();
        m_JointMatrices.resize(m_NumJoints);
        SetJoints(joints);
    }

    // Explicit template instantiations for common cereal archives
    template void Joint::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void Joint::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const);
    template void Joint::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
    template void Joint::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const);
}