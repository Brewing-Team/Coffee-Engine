#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/UUID.h"

#include <cereal/cereal.hpp>
#include <glm/fwd.hpp>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/blending_job.h>

namespace Coffee {

    class AnimationLayer;
    class AnimationController;
    class Skeleton;
    class AnimationSystem;

}

namespace Coffee
{
    /**
     * @brief Component representing an animator.
     * @ingroup scene
     */
    struct AnimatorComponent
    {
        AnimatorComponent() = default;

        /**
         * @brief Copy constructor for AnimatorComponent.
         * @param other The other AnimatorComponent to copy from.
         */
        AnimatorComponent(const AnimatorComponent& other);

        /**
         * @brief Constructs an AnimatorComponent with the given skeleton, animation controller, and animation system.
         * @param skeleton The skeleton reference.
         * @param animationController The animation controller reference.
         */
        AnimatorComponent(Ref<Skeleton> skeleton, Ref<AnimationController> animationController);

        /**
         * @brief Gets the skeleton reference.
         * @return The skeleton reference.
         */
        Ref<Skeleton> GetSkeleton() const { return m_Skeleton; }

        /**
         * @brief Sets the skeleton reference.
         * @param skeleton The skeleton reference to set.
         */
        void SetSkeleton(Ref<Skeleton> skeleton) { m_Skeleton = std::move(skeleton); }

        /**
         * @brief Gets the animation controller reference.
         * @return The animation controller reference.
         */
        Ref<AnimationController> GetAnimationController() const { return m_AnimationController; }

        /**
         * @brief Sets the animation controller reference.
         * @param animationController The animation controller reference to set.
         */
        void SetAnimationController(Ref<AnimationController> animationController)
        {
            m_AnimationController = std::move(animationController);
        }

        /**
         * @brief Gets the sampling job context.
         * @return The sampling job context.
         */
        ozz::animation::SamplingJob::Context& GetContext() { return m_Context; }

        /**
         * @brief Gets the blend layers.
         * @return The blend layers.
         */
        ozz::animation::BlendingJob::Layer* GetBlendLayers() { return m_BlendLayers; }

        /**
         * @brief Gets the blending job.
         * @return The blending job.
         */
        ozz::animation::BlendingJob& GetBlendJob() { return m_BlendJob; }

        /**
         * @brief Sets the current animation for both upper and lower body layers.
         * @param index The index of the animation to set.
         */
        void SetCurrentAnimation(unsigned int index);

        /**
         * @brief Sets the current animation for the upper body layer.
         * @param index The index of the animation to set.
         */
        void SetUpperAnimation(unsigned int index);

        /**
         * @brief Sets the current animation for the lower body layer.
         * @param index The index of the animation to set.
         */
        void SetLowerAnimation(unsigned int index);

        /**
         * @brief Serializes the AnimatorComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template <class Archive> void save(Archive& archive, const std::uint32_t& version) const;

        /**
         * @brief Deserializes the AnimatorComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to deserialize from.
         */
        template <class Archive> void load(Archive& archive, const std::uint32_t& version);

      public:
        bool Loop = true;            ///< Indicates if the animation should loop.
        float BlendDuration = 0.25f; ///< The duration of the blend.
        float AnimationSpeed = 1.0f; ///< The speed of the animation.

        std::vector<glm::mat4> JointMatrices;                    ///< The joint matrices.
        UUID modelUUID;                                          ///< The UUID of the model.
        UUID animatorUUID;                                       ///< The UUID of the animator.
        int UpperBodyRootJoint = 0;                              ///< Index of the root joint for upper body animations.
        std::vector<ozz::math::SoaTransform> PartialBlendOutput; ///< Output transforms for partial blending.

        float UpperBodyWeight = 1.0f;        ///< Weight for blending upper body animations.
        float LowerBodyWeight = 1.0f;        ///< Weight for blending lower body animations.
        float PartialBlendThreshold = 0.01f; ///< Threshold for partial blending.

        Ref<AnimationLayer> UpperAnimation; ///< Animation layer for upper body animations.
        Ref<AnimationLayer> LowerAnimation; ///< Animation layer for lower body animations.

        bool NeedsUpdate = true; ///< Flag to indicate if the animator needs an update.

      private:
        Ref<Skeleton> m_Skeleton;                       ///< The skeleton reference.
        Ref<AnimationController> m_AnimationController; ///< The animation controller reference.

        ozz::animation::SamplingJob::Context m_Context;      ///< The sampling job context.
        ozz::animation::BlendingJob::Layer m_BlendLayers[2]; ///< The blend layers.
        ozz::animation::BlendingJob m_BlendJob;              ///< The blending job.
    };
}

CEREAL_CLASS_VERSION(Coffee::AnimatorComponent, 0);
