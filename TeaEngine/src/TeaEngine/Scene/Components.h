#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Material.h"
#include "TeaEngine/Renderer/Model.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Tea {
    struct TransformComponent
    {
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& position)
            : Position(position) {}

        glm::mat4 GetTransform() const 
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Position)
                    * rotation
                    * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    struct ModelComponent
    {
        Ref<Model> model;

        ModelComponent() = default;
        ModelComponent(const ModelComponent&) = default;
        ModelComponent(Ref<Model> model)
            : model(model) {}
         ModelComponent(const std::string& path)
            :model(CreateRef<Model>(path)) {}
    };

    struct MaterialComponent
    {
        Ref<Material> material;

        MaterialComponent() = default;
        MaterialComponent(const MaterialComponent&) = default;
        MaterialComponent(Ref<Material> material)
            : material(material) {}
    };
}