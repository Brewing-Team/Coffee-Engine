#pragma once

#include "Animation.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "Skeleton.h"
#include "glad/glad.h"

#include <glm/gtc/type_ptr.hpp>
#include <ozz/animation/runtime/local_to_model_job.h>
#include <ozz/animation/runtime/sampling_job.h>
#include <ozz/animation/runtime/blending_job.h>

namespace Coffee {

    class AnimationSystem
    {
    public:
        AnimationSystem();
        ~AnimationSystem() = default;

        void Update(float deltaTime);
        void SetCurrentAnimation(const std::string& name);
        void SetCurrentAnimation(unsigned int index);

        float GetCurrentTime() const { return m_AnimationTime; }
        unsigned int GetCurrentAnimationIndex() const { return m_CurrentAnimation; }

        void SetSkeleton(Ref<Skeleton> skeleton) { m_Skeleton = skeleton; }
        Ref<Skeleton> GetSkeleton() const { return m_Skeleton; }

        void SetAnimationController(Ref<AnimationController> controller) { m_AnimationController = controller; }
        Ref<AnimationController> GetAnimationController() const { return m_AnimationController; }
        void SetBoneTransformations(const Ref<Shader>& shader);

    private:
        void SampleAnimation(float deltaTime, const ozz::animation::Animation& animation, const ozz::animation::Skeleton& skeleton, const std::vector<Joint>& joints, std::vector<glm::mat4>& jointMatrices);
        void BlendAnimations(float deltaTime, const ozz::animation::Skeleton& skeleton, const std::vector<Joint>& joints, std::vector<glm::mat4>& jointMatrices);

        void PrintFloat4x4(const ozz::math::Float4x4& matrix);

        static glm::mat4 OzzToGlmMat4(const ozz::math::Float4x4& from) {
            glm::mat4 to;
            memcpy(glm::value_ptr(to), &from.cols[0], sizeof(glm::mat4));
            return to;
        }

    private:
        GLuint boneMatricesUBO;
        Ref<Skeleton> m_Skeleton;
        Ref<AnimationController> m_AnimationController;
        ozz::animation::SamplingJob::Context m_Context;
        ozz::animation::BlendingJob::Layer m_BlendLayers[2];
        ozz::animation::BlendingJob m_BlendJob;
        unsigned int m_CurrentAnimation = 0;
        unsigned int m_NextAnimation = 0;
        float m_AnimationTime = 0.f;
        float m_BlendTime = 0.f;
        float m_BlendDuration = 0.25f;
        bool m_IsBlending = false;
    };
}