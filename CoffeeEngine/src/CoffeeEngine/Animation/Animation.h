#pragma once

#include <ozz/animation/runtime/animation.h>
#include <ozz/base/memory/unique_ptr.h>

#include <string>
#include <map>
#include <vector>

namespace Coffee {

    class Animation
    {
    public:
        Animation() = default;
        ~Animation() = default;

        Animation(const Animation&) = delete;
        Animation& operator=(const Animation&) = delete;

        Animation(Animation&& other) noexcept
            : m_Name(std::move(other.m_Name))
            , m_Animation(std::move(other.m_Animation))
        {}

        Animation& operator=(Animation&& other) noexcept {
            m_Name = std::move(other.m_Name);
            m_Animation = std::move(other.m_Animation);
            return *this;
        }

        const std::string& GetAnimationName() const { return m_Name; }
        float GetDuration() const { return m_Animation->duration(); }

        void SetAnimation(ozz::unique_ptr<ozz::animation::Animation> animation);
        const ozz::animation::Animation* GetAnimation() const { return m_Animation.get(); }

    private:
        std::string m_Name;
        ozz::unique_ptr<ozz::animation::Animation> m_Animation;
    };

    class AnimationController
    {
    public:
        AnimationController() = default;
        ~AnimationController() = default;

        void AddAnimation(const std::string& name, ozz::unique_ptr<ozz::animation::Animation> animation);
        Animation* GetAnimation(const std::string& name);
        Animation* GetAnimation(unsigned int index);

        unsigned int GetAnimationCount() const { return m_Animations.size(); }
        const std::map<std::string, unsigned int>& GetAnimationMap() const { return m_AnimationsMap; }

    private:
        std::vector<Animation> m_Animations;
        std::map<std::string, unsigned int> m_AnimationsMap;
    };

} // Coffee