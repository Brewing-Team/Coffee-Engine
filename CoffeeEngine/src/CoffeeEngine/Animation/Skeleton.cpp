#include "Skeleton.h"

namespace Coffee {

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
}