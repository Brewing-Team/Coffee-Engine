#pragma once

#include "ozz/base/maths/soa_transform.h"

#include <ozz/animation/runtime/animation.h>
#include <ozz/base/memory/unique_ptr.h>

#include <cereal/cereal.hpp>

#include <string>
#include <map>
#include <vector>

namespace Coffee {
    
    /**
     * @brief Struct representing a layer of animation blending.
     */
    struct AnimationLayer
    {
        bool IsBlending = false; ///< Indicates if blending is currently active.
        unsigned int CurrentAnimation = 0; ///< Index of the current animation.
        unsigned int NextAnimation = 0; ///< Index of the next animation to blend to.
        float BlendTime = 0.0f; ///< Time duration for blending between animations.
        float AnimationTime = 0.0f; ///< Current time position in the current animation.
        float NextAnimationTime = 0.0f; ///< Current time position in the next animation.
        std::vector<ozz::math::SoaTransform> LocalTransforms; ///< Local transforms for the animation joints.
        std::vector<ozz::math::SimdFloat4> JointWeights; ///< Weights for blending animation joints.

        template<class Archive> void serialize(Archive& archive, std::uint32_t const version);
    };

    /**
     * @brief Class representing an animation.
     */
    class AnimationClip // TODO: Rename to AnimationClip or something similar to avoid confusion with ozz::animation::Animation
    {
    public:
        AnimationClip() = default;
        ~AnimationClip() = default;

        AnimationClip(const AnimationClip&) = delete;
        AnimationClip& operator=(const AnimationClip&) = delete;

        AnimationClip(AnimationClip&& other) noexcept
            : m_Name(std::move(other.m_Name))
            , m_Animation(std::move(other.m_Animation))
        {}

        AnimationClip& operator=(AnimationClip&& other) noexcept {
            m_Name = std::move(other.m_Name);
            m_Animation = std::move(other.m_Animation);
            return *this;
        }

        /**
         * @brief Gets the name of the animation.
         * @return The name of the animation.
         */
        const std::string& GetAnimationName() const { return m_Name; }

        /**
         * @brief Gets the duration of the animation.
         * @return The duration of the animation.
         */
        float GetDuration() const { return m_Animation->duration(); }

        /**
         * @brief Sets the animation.
         * @param animation The animation to set.
         */
        void SetAnimation(ozz::unique_ptr<ozz::animation::Animation> animation);

        /**
         * @brief Gets the animation.
         * @return The animation.
         */
        const ozz::animation::Animation* GetAnimation() const { return m_Animation.get(); }

        /**
         * @brief Saves the animation to an archive.
         * @param archive The archive to save to.
         */
        void Save(ozz::io::OArchive& archive) const;

        /**
         * @brief Loads the animation from an archive.
         * @param archive The archive to load from.
         */
        void Load(ozz::io::IArchive& archive);

    private:
        std::string m_Name; ///< The name of the animation.
        ozz::unique_ptr<ozz::animation::Animation> m_Animation; ///< The animation.
    };

    /**
     * @brief Class representing an animation controller.
     */
    class AnimationController
    {
    public:
        AnimationController() = default;
        ~AnimationController() = default;

        /**
         * @brief Adds an animation to the controller.
         * @param name The name of the animation.
         * @param animation The animation to add.
         */
        void AddAnimation(const std::string& name, ozz::unique_ptr<ozz::animation::Animation> animation);

        /**
         * @brief Gets an animation by name.
         * @param name The name of the animation.
         * @return A pointer to the animation.
         */
        AnimationClip* GetAnimationClip(const std::string& name);

        /**
         * @brief Gets an animation by index.
         * @param index The index of the animation.
         * @return A pointer to the animation.
         */
        AnimationClip* GetAnimationClip(unsigned int index);

        /**
         * @brief Gets the number of animations.
         * @return The number of animations.
         */
        unsigned int GetAnimationClipCount() const { return m_AnimationClips.size(); }

        /**
         * @brief Gets the animation map.
         * @return The animation map.
         */
        const std::map<std::string, unsigned int>& GetAnimationClipMap() const { return m_AnimationClipsMap; }

        /**
         * @brief Gets the animations.
         * @return The animations.
         */
        const std::vector<AnimationClip>& GetAnimationClips() const { return m_AnimationClips; }

    private:
        std::vector<AnimationClip> m_AnimationClips; ///< The animations.
        std::map<std::string, unsigned int> m_AnimationClipsMap; ///< The animation map.
    };

} // Coffee