#pragma once

#include <ozz/animation/runtime/skeleton.h>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/base/memory/unique_ptr.h>
#include <glm/mat4x4.hpp>


#include <string>
#include <vector>

namespace cereal {
    class access;
}

namespace Coffee {

    struct Joint
    {
        std::string name;
        int parentIndex;
        ozz::math::Transform localTransform;
        glm::mat4 invBindPose;

    private:
        friend class cereal::access;

        template<class Archive>
        void serialize(Archive& archive)
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
    };

    class Skeleton
    {
    public:
        Skeleton() = default;
        ~Skeleton() = default;

        void SetSkeleton(ozz::unique_ptr<ozz::animation::Skeleton> skeleton);
        const ozz::animation::Skeleton* GetSkeleton() const { return m_Skeleton.get(); }

        void SetJoints(const std::vector<Joint>& joints);
        const std::vector<Joint>& GetJoints() const { return m_Joints; }

        unsigned int GetNumJoints() const { return m_NumJoints; }
        const std::vector<glm::mat4>& GetJointMatrices() const { return m_JointMatrices; }

        void Save(ozz::io::OArchive& archive) const;
        void Load(ozz::io::IArchive& archive, std::vector<Joint>& joints);

    private:
        ozz::unique_ptr<ozz::animation::Skeleton> m_Skeleton;
        std::vector<Joint> m_Joints;
        unsigned int m_NumJoints = 0;
        std::vector<glm::mat4> m_JointMatrices;
    };

} // Coffee
