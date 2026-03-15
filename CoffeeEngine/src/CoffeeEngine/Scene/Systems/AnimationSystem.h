#pragma once

#include "CoffeeEngine/Core/Base.h"

#include <glm/gtc/type_ptr.hpp>
#include <ozz/base/maths/soa_transform.h>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/blending_job.h>

#include <vector>

namespace Coffee {
    class AnimationClip;
    struct AnimationLayer;
    struct AnimatorComponent;
    class Shader;
    class ResourceManager;

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
        void Update(float deltaTime, AnimatorComponent* animator);

        /**
         * @brief Sets the bone transformations for the shader.
         * @param shader The shader to set the bone transformations for.
         * @param animator The animator component.
         */
        void SetBoneTransformations(const Ref<Shader>& shader, const AnimatorComponent* animator);

        /**
         * @brief Sets the current animation for a specific layer.
         * @param index The index of the animation to set.
         * @param animator The animator component.
         * @param layer The animation layer to update.
         */
        void SetCurrentAnimation(unsigned int index, AnimatorComponent* animator, AnimationLayer* layer);

        /**
         * @brief Adds an animator component to the system.
         * @param animatorComponent The animator component to add.
         */
        void AddAnimator(AnimatorComponent* animatorComponent);

        /**
         * @brief Gets the list of animators.
         * @return A vector of animator components.
         */
        std::vector<AnimatorComponent*> GetAnimators() { return m_Animators; }

        /**
         * @brief Resets the animators vector.
         */
        void ResetAnimators() { m_Animators.clear(); }

        /**
         * @brief Sets the resource manager used for loading resources.
         * @param manager Pointer to the ResourceManager instance.
         */
        void SetResourceManager(ResourceManager* manager) { m_ResourceManager = manager; }

        /**
         * @brief Loads the animator.
         * @param animator The animator component.
         */
        void LoadAnimator(AnimatorComponent* animator);

        /**
         * @brief Sets up partial blending for upper and lower body animations.
         * @param upperBodyAnimIndex The index of the upper body animation.
         * @param lowerBodyAnimIndex The index of the lower body animation.
         * @param upperBodyJointName The name of the upper body root joint.
         * @param animator The animator component.
         */
        void SetupPartialBlending(unsigned int upperBodyAnimIndex, unsigned int lowerBodyAnimIndex, const std::string& upperBodyJointName, AnimatorComponent* animator);

    private:
        /**
         * @brief Updates blending for a specific animation layer.
         * @param deltaTime The time elapsed since the last update.
         * @param animator The animator component.
         * @param layer The animation layer to update.
         */
        void UpdateBlending(float deltaTime, const AnimatorComponent* animator, AnimationLayer* layer);

        /**
         * @brief Blends transforms between two sets of animations.
         * @param currentTransforms The current animation transforms.
         * @param nextTransforms The next animation transforms.
         * @param blendRatio The ratio for blending between the two animations.
         */
        void BlendTransforms(std::vector<ozz::math::SoaTransform>& currentTransforms, const std::vector<ozz::math::SoaTransform>& nextTransforms, float blendRatio);

        /**
         * @brief Updates partial blending for upper and lower body animations.
         * @param deltaTime The time elapsed since the last update.
         * @param animator The animator component.
         */
        void UpdatePartialBlending(float deltaTime, AnimatorComponent* animator);

        /**
         * @brief Sets up per-joint weights for partial blending.
         * @param animator The animator component.
         * @param upperBodyRootIndex The index of the upper body root joint.
         */
        void SetupPerJointWeights(const AnimatorComponent* animator, int upperBodyRootIndex);

        /**
         * @brief Updates the animation times for a specific layer.
         * @param deltaTime The time elapsed since the last update.
         * @param animator The animator component.
         * @param layer The animation layer to update.
         * @param currentAnim The current animation.
         */
        void UpdateLayerTimes(float deltaTime, const AnimatorComponent* animator, AnimationLayer* layer, const AnimationClip* currentAnim);

        /**
         * @brief Samples and blends animations for a specific layer.
         * @param animator The animator component.
         * @param layer The animation layer.
         * @param currentAnim The current animation.
         * @param nextAnim The next animation.
         * @param outputTransforms The output transforms for the layer.
         */
        void SampleAndBlendLayerAnimations(AnimatorComponent* animator, AnimationLayer* layer, const AnimationClip* currentAnim, const AnimationClip* nextAnim, std::vector<ozz::math::SoaTransform>& outputTransforms);

        /**
         * @brief Samples the transforms for the animation.
         * @param animator The animator component.
         * @param animationIndex The index of the animation.
         * @param timeRatio The time ratio for the animation.
         * @return A vector of sampled transforms.
         */
        std::vector<ozz::math::SoaTransform> SampleTransforms(AnimatorComponent* animator, unsigned int animationIndex, float timeRatio);

        /**
         * @brief Converts local transforms to model space.
         * @param animator The animator component.
         * @param localTransforms The local transforms.
         * @return A vector of transforms in model space.
         */
        std::vector<ozz::math::Float4x4> ConvertToModelSpace(AnimatorComponent* animator, const std::vector<ozz::math::SoaTransform>& localTransforms);

        /**
         * @brief Converts an Ozz matrix to a GLM matrix.
         * @param from The Ozz matrix.
         * @return The GLM matrix.
         */
        glm::mat4 OzzToGlmMat4(const ozz::math::Float4x4& from) {
            glm::mat4 to;
            memcpy(glm::value_ptr(to), &from.cols[0], sizeof(glm::mat4));
            return to;
        }

    private:
        // When refactoring this system this should not exist anymore...
        std::vector<AnimatorComponent*> m_Animators; ///< The list of animator components.
        ResourceManager* m_ResourceManager = nullptr; ///< Resource manager for loading model resources.
    };
} // namespace Coffee
