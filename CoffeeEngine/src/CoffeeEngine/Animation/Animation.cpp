#include "Animation.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace Coffee {

    // AnimationLayer implementation
    template<class Archive>
    void AnimationLayer::serialize(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("CurrentAnimation", CurrentAnimation));
    }

    void Animation::SetAnimation(ozz::unique_ptr<ozz::animation::Animation> animation)
    {
        m_Animation = std::move(animation);
        m_Name = m_Animation->name();
    }

    void Animation::Save(ozz::io::OArchive& archive) const
    {
        m_Animation->Save(archive);
    }

    void Animation::Load(ozz::io::IArchive& archive)
    {
        m_Animation->Load(archive, 7);
        m_Name = m_Animation->name();
    }

    void AnimationController::AddAnimation(const std::string& name, ozz::unique_ptr<ozz::animation::Animation> animation)
    {
        m_AnimationsMap[name] = m_Animations.size();
        Animation newAnimation;
        newAnimation.SetAnimation(std::move(animation));
        m_Animations.push_back(std::move(newAnimation));
    }

    Animation* AnimationController::GetAnimation(const std::string& name)
    {
        auto it = m_AnimationsMap.find(name);
        if (it != m_AnimationsMap.end())
            return &m_Animations[it->second];

        return nullptr;
    }

    Animation* AnimationController::GetAnimation(unsigned int index)
    {
        if (index < m_Animations.size())
            return &m_Animations[index];

        return nullptr;
    }

    // Explicit template instantiations for common cereal archives
    template void AnimationLayer::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void AnimationLayer::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const);
    template void AnimationLayer::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
    template void AnimationLayer::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const);
}