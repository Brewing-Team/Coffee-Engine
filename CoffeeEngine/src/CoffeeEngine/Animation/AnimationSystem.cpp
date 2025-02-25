#include "AnimationSystem.h"

#include <iostream>

namespace Coffee {

    AnimationSystem::AnimationSystem()
    {
        m_BlendJob.layers = ozz::make_span(m_BlendLayers);
    }

    void AnimationSystem::Update(float deltaTime)
    {
        if (!m_AnimationController || !m_Skeleton) return;

        if (m_IsBlending)
        {
            m_BlendTime += deltaTime;

            if (m_BlendTime >= m_BlendDuration)
            {
                m_CurrentAnimation = m_NextAnimation;
                m_IsBlending = false;
            }

            BlendAnimations(deltaTime, *m_Skeleton->GetSkeleton(), m_Skeleton->GetJoints(), const_cast<std::vector<glm::mat4>&>(m_Skeleton->GetJointMatrices()));
            return;
        }

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

        for (size_t i = 0; i < modelSpaceTransforms.size(); ++i)
        {
            jointMatrices[i] = OzzToGlmMat4(modelSpaceTransforms[i]) * joints[i].invBindPose;
        }
    }

    void AnimationSystem::BlendAnimations(float deltaTime, const ozz::animation::Skeleton& skeleton, const std::vector<Joint>& joints, std::vector<glm::mat4>& jointMatrices)
    {
        Animation* currentAnim = m_AnimationController->GetAnimation(m_CurrentAnimation);
        Animation* nextAnim = m_AnimationController->GetAnimation(m_NextAnimation);

        if (!currentAnim || !nextAnim)
        {
            std::cerr << "Invalid animations" << std::endl;
            return;
        }

        const ozz::animation::Animation* currentAnimation = currentAnim->GetAnimation();
        const ozz::animation::Animation* nextAnimation = nextAnim->GetAnimation();

        if (!currentAnimation || !nextAnimation)
        {
            std::cerr << "Invalid animation data" << std::endl;
            return;
        }

        const int numJoints = skeleton.num_joints();

        std::vector<ozz::math::SoaTransform> localTransforms1(numJoints);
        std::vector<ozz::math::SoaTransform> localTransforms2(numJoints);
        std::vector<ozz::math::SoaTransform> blendedTransforms(numJoints);

        ozz::animation::SamplingJob samplingJob1;
        samplingJob1.animation = currentAnimation;
        samplingJob1.context = &m_Context;
        samplingJob1.ratio = m_AnimationTime / currentAnimation->duration();
        samplingJob1.output = ozz::make_span(localTransforms1);

        if (!samplingJob1.Run())
        {
            std::cerr << "Failed to sample current animation" << std::endl;
            return;
        }

        ozz::animation::SamplingJob samplingJob2;
        samplingJob2.animation = nextAnimation;
        samplingJob2.context = &m_Context;
        samplingJob2.ratio = m_AnimationTime / nextAnimation->duration();
        samplingJob2.output = ozz::make_span(localTransforms2);

        if (!samplingJob2.Run())
        {
            std::cerr << "Failed to sample next animation" << std::endl;
            return;
        }

        m_BlendLayers[0].transform = ozz::make_span(localTransforms1);
        m_BlendLayers[0].weight = 1.0f - (m_BlendTime / m_BlendDuration);
        m_BlendLayers[1].transform = ozz::make_span(localTransforms2);
        m_BlendLayers[1].weight = m_BlendTime / m_BlendDuration;

        m_BlendJob.rest_pose = ozz::make_span(m_Skeleton->GetSkeleton()->joint_rest_poses());
        m_BlendJob.output = ozz::make_span(blendedTransforms);

        if (!m_BlendJob.Run())
        {
            std::cerr << "Failed to blend animations" << std::endl;
            return;
        }

        std::vector<ozz::math::Float4x4> modelSpaceTransforms(numJoints);

        ozz::animation::LocalToModelJob localToModelJob;
        localToModelJob.skeleton = &skeleton;
        localToModelJob.input = ozz::make_span(blendedTransforms);
        localToModelJob.output = ozz::make_span(modelSpaceTransforms);

        if (!localToModelJob.Run())
        {
            std::cerr << "Failed to convert local to model transforms" << std::endl;
            std::fill(jointMatrices.begin(), jointMatrices.end(), glm::mat4(1.0f));
            return;
        }

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
            m_NextAnimation = it->second;
            m_BlendTime = 0.f;
            m_IsBlending = true;
            m_Context.Resize(GetAnimationController()->GetAnimation(it->first)->GetAnimation()->num_tracks());
        }
    }

    void AnimationSystem::SetCurrentAnimation(unsigned int index)
    {
        if (!m_AnimationController) return;

        if (index < m_AnimationController->GetAnimationCount())
        {
            m_NextAnimation = index;
            m_BlendTime = 0.f;
            m_IsBlending = true;
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