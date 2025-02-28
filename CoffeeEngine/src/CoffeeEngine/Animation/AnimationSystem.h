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

    class AnimationSystem
    {
    public:
        AnimationSystem() = default;
        ~AnimationSystem() = default;

        void Update(float deltaTime, AnimatorComponent* animator);
        void SetCurrentAnimation(const std::string& name, AnimatorComponent* animator);
        void SetCurrentAnimation(unsigned int index, AnimatorComponent* animator);

        void SetBoneTransformations(const Ref<Shader>& shader, AnimatorComponent* animator);

    private:
        void SampleAnimation(float deltaTime, AnimatorComponent* animator);
        void BlendAnimations(float deltaTime, AnimatorComponent* animator);

        static glm::mat4 OzzToGlmMat4(const ozz::math::Float4x4& from) {
            glm::mat4 to;
            memcpy(glm::value_ptr(to), &from.cols[0], sizeof(glm::mat4));
            return to;
        }
    };
} // namespace Coffee
