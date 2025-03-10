#include "AnimationSystem.h"

#define BT_NO_SIMD_OPERATOR_OVERLOADS

#include "CoffeeEngine/Scene/Components.h"

#include <iostream>

namespace Coffee {

    void AnimationSystem::Update(float deltaTime, AnimatorComponent* animator)
    {
        Ref<Skeleton> animatorSkeleton = animator->GetSkeleton();

        if (animator->IsBlending)
        {
            animator->BlendTime += deltaTime * animator->AnimationSpeed;

            if (animator->BlendTime >= animator->BlendDuration)
            {
                animator->CurrentAnimation = animator->NextAnimation;
                animator->AnimationTime = animator->NextAnimationTime;
                animator->IsBlending = false;
            }

            if (animator->IsBlending)
            {
                BlendAnimations(deltaTime, animator);
                return;
            }
        }

        if (!animator->GetAnimationController()->GetAnimation(animator->CurrentAnimation)) return;

        SampleAnimation(deltaTime, animator);
    }

    void AnimationSystem::SampleAnimation(float deltaTime, AnimatorComponent* animator)
    {
        animator->AnimationTime += deltaTime * animator->AnimationSpeed;
        float animationDuration = animator->GetAnimationController()->GetAnimation(animator->CurrentAnimation)->GetAnimation()->duration();
        if (animator->AnimationTime > animationDuration)
            animator->AnimationTime = std::fmod(animator->AnimationTime, animationDuration);

        std::vector<ozz::math::SoaTransform> localTransforms = SampleTransforms(animator, animator->CurrentAnimation, animator->AnimationTime / animationDuration);

        std::vector<ozz::math::Float4x4> modelSpaceTransforms = ConvertToModelSpace(animator, localTransforms);

        for (size_t i = 0; i < modelSpaceTransforms.size(); ++i)
        {
            animator->JointMatrices[i] = OzzToGlmMat4(modelSpaceTransforms[i]) * animator->GetSkeleton()->GetJoints()[i].invBindPose;
        }
    }

    std::vector<ozz::math::SoaTransform> AnimationSystem::SampleTransforms(AnimatorComponent* animator, unsigned int animationIndex, float timeRatio)
    {
        const int numJoints = animator->GetSkeleton()->GetSkeleton()->num_joints();
        std::vector<ozz::math::SoaTransform> localTransforms(numJoints);

        ozz::animation::SamplingJob samplingJob;
        samplingJob.animation = animator->GetAnimationController()->GetAnimation(animationIndex)->GetAnimation();
        samplingJob.context = &animator->GetContext();
        samplingJob.ratio = timeRatio;
        samplingJob.output = ozz::make_span(localTransforms);

        if (!samplingJob.Run())
        {
            std::cerr << "Failed to sample animation" << std::endl;
        }

        return localTransforms;
    }

    std::vector<ozz::math::Float4x4> AnimationSystem::ConvertToModelSpace(AnimatorComponent* animator, const std::vector<ozz::math::SoaTransform>& localTransforms)
    {
        const int numJoints = animator->GetSkeleton()->GetSkeleton()->num_joints();
        std::vector<ozz::math::Float4x4> modelSpaceTransforms(numJoints);

        ozz::animation::LocalToModelJob localToModelJob;
        localToModelJob.skeleton = animator->GetSkeleton()->GetSkeleton();
        localToModelJob.input = ozz::make_span(localTransforms);
        localToModelJob.output = ozz::make_span(modelSpaceTransforms);

        if (!localToModelJob.Run())
        {
            std::cerr << "Failed to convert local to model transforms" << std::endl;
            std::fill(animator->JointMatrices.begin(), animator->JointMatrices.end(), glm::mat4(1.0f));
        }

        return modelSpaceTransforms;
    }

    void AnimationSystem::BlendAnimations(float deltaTime, AnimatorComponent* animator)
    {
        Animation* currentAnim = animator->GetAnimationController()->GetAnimation(animator->CurrentAnimation);
        Animation* nextAnim = animator->GetAnimationController()->GetAnimation(animator->NextAnimation);

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

        const int numJoints = animator->GetSkeleton()->GetSkeleton()->num_joints();

        std::vector<ozz::math::SoaTransform> localTransforms1 = SampleTransforms(animator, animator->CurrentAnimation,
            fmod(animator->AnimationTime, currentAnimation->duration()) / currentAnimation->duration());
        std::vector<ozz::math::SoaTransform> localTransforms2 = SampleTransforms(animator, animator->NextAnimation,
            fmod(animator->NextAnimationTime, nextAnimation->duration()) / nextAnimation->duration());
        std::vector<ozz::math::SoaTransform> blendedTransforms(numJoints);

        animator->AnimationTime += deltaTime * animator->AnimationSpeed;
        animator->NextAnimationTime += deltaTime * animator->AnimationSpeed;

        float blendFactor = glm::clamp(animator->BlendTime / animator->BlendDuration, 0.0f, 1.0f);

        animator->GetBlendLayers()[0].transform = ozz::make_span(localTransforms1);
        animator->GetBlendLayers()[0].weight = 1.0f - blendFactor;
        animator->GetBlendLayers()[1].transform = ozz::make_span(localTransforms2);
        animator->GetBlendLayers()[1].weight = blendFactor;

        animator->GetBlendJob().rest_pose = ozz::make_span(animator->GetSkeleton()->GetSkeleton()->joint_rest_poses());
        animator->GetBlendJob().output = ozz::make_span(blendedTransforms);
        animator->GetBlendJob().threshold = animator->BlendThreshold;

        if (!animator->GetBlendJob().Run())
        {
            std::cerr << "Failed to blend animations" << std::endl;
            return;
        }

        std::vector<ozz::math::Float4x4> modelSpaceTransforms = ConvertToModelSpace(animator, blendedTransforms);

        for (size_t i = 0; i < modelSpaceTransforms.size(); ++i)
        {
            animator->JointMatrices[i] = OzzToGlmMat4(modelSpaceTransforms[i]) * animator->GetSkeleton()->GetJoints()[i].invBindPose;
        }
    }

    void AnimationSystem::SetCurrentAnimation(const std::string& name, AnimatorComponent* animator)
    {
        if (!animator->GetAnimationController()) return;

        auto& animMap = animator->GetAnimationController()->GetAnimationMap();
        auto it = animMap.find(name);
        if (it != animMap.end())
        {
            animator->NextAnimation = it->second;
            animator->BlendTime = 0.f;
            animator->IsBlending = true;
            animator->GetContext().Resize(animator->GetAnimationController()->GetAnimation(it->first)->GetAnimation()->num_tracks());
        }
    }

    void AnimationSystem::SetCurrentAnimation(unsigned int index, AnimatorComponent* animator)
    {
        if (!animator->GetAnimationController()) return;

        if (index < animator->GetAnimationController()->GetAnimationCount())
        {
            animator->NextAnimation = index;
            animator->BlendTime = 0.f;
            animator->IsBlending = true;
            animator->GetContext().Resize(animator->GetAnimationController()->GetAnimation(index)->GetAnimation()->num_tracks());
        }
    }

    void AnimationSystem::SetBoneTransformations(const Ref<Shader>& shader, AnimatorComponent* animator)
    {
        shader->setBool("animated", true);

        const std::vector<glm::mat4>& jointMatrices = animator->JointMatrices;
        shader->setMat4v("finalBonesMatrices", jointMatrices);
    }

    void AnimationSystem::AddAnimator(AnimatorComponent* animatorComponent)
    {
        m_Animators.push_back(animatorComponent);
    }
}