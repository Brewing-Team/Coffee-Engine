#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Material.h"
#include "TeaEngine/Renderer/Mesh.h"
#include "TeaEngine/Renderer/Model.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Tea {
    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
    };

    struct TransformComponent
    {
        private:
        glm::mat4 worldMatrix = glm::mat4(1.0f);
        public:
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& position)
            : Position(position) {}

        glm::mat4 GetLocalTransform() const 
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Position)
                    * rotation
                    * glm::scale(glm::mat4(1.0f), Scale);
        }

        void SetLocalTransform(const glm::mat4& transform) //TODO: Improve this function, this way is ugly and glm::decompose is from gtx (is supposed to not be very stable)
        {
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::quat orientation;

            glm::decompose(transform, Scale, orientation, Position, skew, perspective);
            Rotation = glm::eulerAngles(orientation);
        }

        const glm::mat4& GetWorldTransform() const
        {
            return worldMatrix;
        }

        void SetWorldTransform(const glm::mat4& transform)
        {
            worldMatrix = transform * GetLocalTransform();
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

    struct MeshComponent
    {
        Ref<Mesh> mesh;

        MeshComponent()
        {
            //TEMPORAL! In the future use for example MeshComponent() : mesh(MeshFactory(PrimitiveType::MeshText))
            Ref<Model> m = CreateRef<Model>("assets/models/MissingMesh.glb");
            mesh = m->GetMeshes()[0];
        }
        MeshComponent(const MeshComponent&) = default;
        MeshComponent(Ref<Mesh> mesh)
            : mesh(mesh) {}

        const Ref<Mesh>& GetMesh() const {return mesh; };
    };

    struct MaterialComponent
    {
        Ref<Material> material;

        MaterialComponent()
            : material(CreateRef<Material>()) {}
        MaterialComponent(const MaterialComponent&) = default;
        MaterialComponent(Ref<Material> material)
            : material(material) {}
    };
}