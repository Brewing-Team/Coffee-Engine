#include "AnimationClip.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee {

    // AnimationLayer implementation
    template<class Archive>
    void AnimationLayer::serialize(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("CurrentAnimation", CurrentAnimation));
    }

    void AnimationClip::SetAnimation(ozz::unique_ptr<ozz::animation::Animation> animation)
    {
        m_Animation = std::move(animation);
        m_Name = m_Animation->name();
    }

    void AnimationClip::Save(ozz::io::OArchive& archive) const
    {
        m_Animation->Save(archive);
    }

    void AnimationClip::Load(ozz::io::IArchive& archive)
    {
        m_Animation->Load(archive, 7);
        m_Name = m_Animation->name();
    }

    void AnimationController::AddAnimation(const std::string& name, ozz::unique_ptr<ozz::animation::Animation> animation)
    {
        m_AnimationClipsMap[name] = m_AnimationClips.size();
        AnimationClip newAnimation;
        newAnimation.SetAnimation(std::move(animation));
        m_AnimationClips.push_back(std::move(newAnimation));
    }

    AnimationClip* AnimationController::GetAnimationClip(const std::string& name)
    {
        auto it = m_AnimationClipsMap.find(name);
        if (it != m_AnimationClipsMap.end())
            return &m_AnimationClips[it->second];

        return nullptr;
    }

    AnimationClip* AnimationController::GetAnimationClip(unsigned int index)
    {
        if (index < m_AnimationClips.size())
            return &m_AnimationClips[index];

        return nullptr;
    }

    // Explicit template instantiations for common cereal archives
    template void AnimationLayer::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void AnimationLayer::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const);
    template void AnimationLayer::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
    template void AnimationLayer::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const);
}