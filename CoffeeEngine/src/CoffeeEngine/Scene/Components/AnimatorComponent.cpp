#include "AnimatorComponent.h"
#include "CoffeeEngine/Animation/Animation.h"
#include "CoffeeEngine/Animation/AnimationSystem.h"
#include "CoffeeEngine/Animation/Skeleton.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>

namespace Coffee 
{
    AnimatorComponent::AnimatorComponent(const AnimatorComponent& other)
            : Loop(other.Loop), BlendDuration(other.BlendDuration), AnimationSpeed(other.AnimationSpeed),
              JointMatrices(other.JointMatrices), modelUUID(other.modelUUID), animatorUUID(other.animatorUUID),
              m_Skeleton(other.m_Skeleton), m_AnimationController(other.m_AnimationController),
              UpperAnimation(other.UpperAnimation), LowerAnimation(other.LowerAnimation),
              PartialBlendThreshold(other.PartialBlendThreshold), UpperBodyWeight(other.UpperBodyWeight),
              LowerBodyWeight(other.LowerBodyWeight), UpperBodyRootJoint(other.UpperBodyRootJoint)
    {
        m_BlendJob.layers = ozz::make_span(m_BlendLayers);
        const std::string rootJointName = GetSkeleton()->GetJoints()[UpperBodyRootJoint].name;
        AnimationSystem::SetupPartialBlending(UpperAnimation->CurrentAnimation, LowerAnimation->CurrentAnimation,
                                                rootJointName, this);
        AnimationSystem::AddAnimator(this);
    }

    AnimatorComponent::AnimatorComponent(Ref<Skeleton> skeleton, Ref<AnimationController> animationController)
            : m_Skeleton(std::move(skeleton)), m_AnimationController(std::move(animationController)),
              UpperAnimation(std::make_shared<AnimationLayer>()), LowerAnimation(std::make_shared<AnimationLayer>())
    {
        m_BlendJob.layers = ozz::make_span(m_BlendLayers);
        JointMatrices = m_Skeleton->GetJointMatrices();
    }

    void AnimatorComponent::SetCurrentAnimation(unsigned int index)
    {
        AnimationSystem::SetCurrentAnimation(index, this, UpperAnimation.get());
        AnimationSystem::SetCurrentAnimation(index, this, LowerAnimation.get());
    }

    void AnimatorComponent::SetUpperAnimation(unsigned int index)
    {
        AnimationSystem::SetCurrentAnimation(index, this, UpperAnimation.get());
    }

    void AnimatorComponent::SetLowerAnimation(unsigned int index)
    {
        AnimationSystem::SetCurrentAnimation(index, this, LowerAnimation.get());
    }


    template <class Archive> 
    void AnimatorComponent::save(Archive& archive, const std::uint32_t& version) const
    {
        archive(cereal::make_nvp("BlendDuration", BlendDuration),
                cereal::make_nvp("AnimationSpeed", AnimationSpeed), cereal::make_nvp("Loop", Loop),
                cereal::make_nvp("ModelUUID", modelUUID), cereal::make_nvp("AnimatorUUID", animatorUUID),
                cereal::make_nvp("UpperAnimation", UpperAnimation),
                cereal::make_nvp("LowerAnimation", LowerAnimation),
                cereal::make_nvp("PartialBlendThreshold", PartialBlendThreshold),
                cereal::make_nvp("UpperBodyWeight", UpperBodyWeight),
                cereal::make_nvp("LowerBodyWeight", LowerBodyWeight),
                cereal::make_nvp("UpperBodyRootJoint", UpperBodyRootJoint));
    }

    template <class Archive> 
    void AnimatorComponent::load(Archive& archive, const std::uint32_t& version)
    {
        archive(cereal::make_nvp("BlendDuration", BlendDuration),
                cereal::make_nvp("AnimationSpeed", AnimationSpeed), cereal::make_nvp("Loop", Loop),
                cereal::make_nvp("ModelUUID", modelUUID), cereal::make_nvp("AnimatorUUID", animatorUUID),
                cereal::make_nvp("UpperAnimation", UpperAnimation),
                cereal::make_nvp("LowerAnimation", LowerAnimation),
                cereal::make_nvp("PartialBlendThreshold", PartialBlendThreshold),
                cereal::make_nvp("UpperBodyWeight", UpperBodyWeight),
                cereal::make_nvp("LowerBodyWeight", LowerBodyWeight),
                cereal::make_nvp("UpperBodyRootJoint", UpperBodyRootJoint));

        AnimationSystem::LoadAnimator(this);
    }

    // Explicit template instantiations for common cereal archives
    template void AnimatorComponent::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, const std::uint32_t&) const;
    template void AnimatorComponent::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&, const std::uint32_t&);
    template void AnimatorComponent::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, const std::uint32_t&) const;
    template void AnimatorComponent::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, const std::uint32_t&);
}
