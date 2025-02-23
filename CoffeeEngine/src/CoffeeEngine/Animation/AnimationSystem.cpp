#include "AnimationSystem.h"

#include <iostream>

namespace Coffee {

    void AnimationSystem::Update(float deltaTime)
    {
        if (!m_AnimationController || !m_Skeleton) return;

        Animation* currentAnim = m_AnimationController->GetAnimation(m_CurrentAnimation);
        if (!currentAnim) return;

        const ozz::animation::Animation* animation = currentAnim->GetAnimation();
        const ozz::animation::Skeleton* skeleton = m_Skeleton->GetSkeleton();

        if (!animation || !skeleton) return;

        SampleAnimation(deltaTime, *animation, *skeleton, m_Skeleton->GetJoints(), const_cast<std::vector<glm::mat4>&>(m_Skeleton->GetJointMatrices()));
    }

    void AnimationSystem::SampleAnimation(float deltaTime, const ozz::animation::Animation& animation, const ozz::animation::Skeleton& skeleton, const std::vector<Joint>& joints, std::vector<glm::mat4>& jointMatrices)
    {
        m_AnimationTime += deltaTime;
        if (m_AnimationTime > animation.duration())
            m_AnimationTime = std::fmod(m_AnimationTime, animation.duration());

        const int numJoints = skeleton.num_joints();

        std::vector<ozz::math::SoaTransform> localTransforms(numJoints);
        ozz::animation::SamplingJob samplingJob;
        samplingJob.animation = &animation;
        samplingJob.context = &m_Context;
        samplingJob.ratio = m_AnimationTime / animation.duration();
        samplingJob.output = ozz::make_span(localTransforms);

        if (!samplingJob.Run())
        {
            std::cerr << "Failed to sample animation" << std::endl;
            return;
        }

        std::vector<ozz::math::Float4x4> modelSpaceTransforms(numJoints);

        ozz::animation::LocalToModelJob localToModelJob;
        localToModelJob.skeleton = &skeleton;
        localToModelJob.input = ozz::make_span(localTransforms);
        localToModelJob.output = ozz::make_span(modelSpaceTransforms);

        if (!localToModelJob.Run())
        {
            std::cerr << "Failed to convert local to model transforms" << std::endl;
            std::fill(jointMatrices.begin(), jointMatrices.end(), glm::mat4(1.0f));
            return;
        }

        // FIXME - Incorrect values in jointMatrices
        for (size_t i = 0; i < modelSpaceTransforms.size(); ++i)
        {
            jointMatrices[i] = OzzToGlmMat4(modelSpaceTransforms[i]) * joints[i].invBindPose;
        }
    }

    void AnimationSystem::SetCurrentAnimation(const std::string& name)
    {
        if (!m_AnimationController) return;
        
        auto& animMap = m_AnimationController->GetAnimationMap();
        auto it = animMap.find(name);
        if (it != animMap.end())
        {
            m_CurrentAnimation = it->second;
            m_AnimationTime = 0.f;
            m_Context.Resize(GetAnimationController()->GetAnimation(it->first)->GetAnimation()->num_tracks());
        }
    }

    void AnimationSystem::SetCurrentAnimation(unsigned int index)
    {
        if (!m_AnimationController) return;
        
        if (index < m_AnimationController->GetAnimationCount())
        {
            m_CurrentAnimation = index;
            m_AnimationTime = 0.f;
            m_Context.Resize(GetAnimationController()->GetAnimation(index)->GetAnimation()->num_tracks());
        }
    }

    void AnimationSystem::SetBoneTransformations(const Ref<Shader>& shader)
    {
        if (!m_AnimationController || !m_Skeleton) return;

        shader->setBool("animated", true);

        const std::vector<glm::mat4>& jointMatrices = m_Skeleton->GetJointMatrices();
        shader->setMat4v("finalBonesMatrices", jointMatrices);
    }
}