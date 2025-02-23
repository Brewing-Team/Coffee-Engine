#include "Animation.h"

namespace Coffee {

    void Animation::SetAnimation(ozz::unique_ptr<ozz::animation::Animation> animation)
    {
        m_Animation = std::move(animation);
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
}