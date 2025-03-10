#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "Skeleton.h"

#include <glm/gtc/type_ptr.hpp>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/blending_job.h>

namespace Coffee {
    struct AnimatorComponent;

    /**
     * @brief System responsible for handling animations.
     */
    class AnimationSystem
    {
    public:

        /**
         * @brief Updates the animation system.
         * @param deltaTime The time elapsed since the last update.
         * @param animator The animator component to update.
         */
        static void Update(float deltaTime, AnimatorComponent* animator);

        /**
         * @brief Sets the current animation by name.
         * @param name The name of the animation.
         * @param animator The animator component.
         */
        static void SetCurrentAnimation(const std::string& name, AnimatorComponent* animator);

        /**
         * @brief Sets the current animation by index.
         * @param index The index of the animation.
         * @param animator The animator component.
         */
        static void SetCurrentAnimation(unsigned int index, AnimatorComponent* animator);

        /**
         * @brief Sets the bone transformations for the shader.
         * @param shader The shader to set the bone transformations for.
         * @param animator The animator component.
         */
        static void SetBoneTransformations(const Ref<Shader>& shader, AnimatorComponent* animator);

        /**
         * @brief Adds an animator component to the system.
         * @param animatorComponent The animator component to add.
         */
        static void AddAnimator(AnimatorComponent* animatorComponent);

        /**
         * @brief Gets the list of animators.
         * @return A vector of animator components.
         */
        static std::vector<AnimatorComponent*> GetAnimators() { return m_Animators; }

        /**
         * @brief Resets the animators vector.
         */
        static void ResetAnimators() { m_Animators.clear(); }

        /**
         * @brief Loads the animator.
         * @param animator The animator component.
         */
        static void LoadAnimator(AnimatorComponent* animator);

    private:
        /**
         * @brief Samples the animation.
         * @param deltaTime The time elapsed since the last update.
         * @param animator The animator component.
         */
        static void SampleAnimation(float deltaTime, AnimatorComponent* animator);

        /**
         * @brief Samples the transforms for the animation.
         * @param animator The animator component.
         * @param animationIndex The index of the animation.
         * @param timeRatio The time ratio for the animation.
         * @return A vector of sampled transforms.
         */
        static std::vector<ozz::math::SoaTransform> SampleTransforms(AnimatorComponent* animator, unsigned int animationIndex, float timeRatio);

        /**
         * @brief Converts local transforms to model space.
         * @param animator The animator component.
         * @param localTransforms The local transforms.
         * @return A vector of transforms in model space.
         */
        static std::vector<ozz::math::Float4x4> ConvertToModelSpace(AnimatorComponent* animator, const std::vector<ozz::math::SoaTransform>& localTransforms);

        /**
         * @brief Blends animations.
         * @param deltaTime The time elapsed since the last update.
         * @param animator The animator component.
         */
        static void BlendAnimations(float deltaTime, AnimatorComponent* animator);

        /**
         * @brief Converts an Ozz matrix to a GLM matrix.
         * @param from The Ozz matrix.
         * @return The GLM matrix.
         */
        static glm::mat4 OzzToGlmMat4(const ozz::math::Float4x4& from) {
            glm::mat4 to;
            memcpy(glm::value_ptr(to), &from.cols[0], sizeof(glm::mat4));
            return to;
        }

    private:
        static std::vector<AnimatorComponent*> m_Animators; ///< The list of animator components.
    };
} // namespace Coffee
