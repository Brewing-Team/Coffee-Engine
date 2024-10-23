#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Material.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/Model.h"
#include "CoffeeEngine/Scene/SceneCamera.h"
#include <cereal/cereal.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

//TEMPORAL
namespace cereal {
    template<class Archive>
    void serialize(Archive& archive, glm::vec2& vec)
    {
        archive(cereal::make_nvp("x", vec.x), cereal::make_nvp("y", vec.y));
    }

    template<class Archive>
    void serialize(Archive& archive, glm::vec3& vec)
    {
        archive(cereal::make_nvp("x", vec.x), cereal::make_nvp("y", vec.y), cereal::make_nvp("z", vec.z));
    }

    template<class Archive>
    void serialize(Archive& archive, glm::vec4& vec)
    {
        archive(cereal::make_nvp("x", vec.x), cereal::make_nvp("y", vec.y), cereal::make_nvp("z", vec.z), cereal::make_nvp("w", vec.w));
    }

    template<class Archive>
    void serialize(Archive& archive, glm::quat& quat)
    {
        archive(cereal::make_nvp("x", quat.x), cereal::make_nvp("y", quat.y), cereal::make_nvp("z", quat.z), cereal::make_nvp("w", quat.w));
    }
}

namespace Coffee {
    struct TagComponent
    {
        std::string Tag;

        TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::make_nvp("Tag", Tag));
        }
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

        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::make_nvp("Position", Position), cereal::make_nvp("Rotation", Rotation), cereal::make_nvp("Scale", Scale));
        }
    };

    struct CameraComponent
    {
        SceneCamera Camera;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::make_nvp("Camera", Camera));
        }
    };

    struct MeshComponent
    {
        Ref<Mesh> mesh;
        bool drawAABB = false;

        MeshComponent()
        {
            //TEMPORAL! In the future use for example MeshComponent() : mesh(MeshFactory(PrimitiveType::MeshText))
            Ref<Model> m = Model::Load("assets/models/MissingMesh.glb");
            mesh = m->GetMeshes()[0];
        }
        MeshComponent(const MeshComponent&) = default;
        MeshComponent(Ref<Mesh> mesh)
            : mesh(mesh) {}

        const Ref<Mesh>& GetMesh() const {return mesh; };

        template<class Archive>
        void save(Archive& archive)
        {
            archive(cereal::make_nvp("Mesh", *mesh));
        }

        template<class Archive>
        void load(Archive& archive)
        {
            archive(*mesh);
        }
    };

    struct MaterialComponent
    {
        Ref<Material> material;

        MaterialComponent()
            : material(CreateRef<Material>()) {}
        MaterialComponent(const MaterialComponent&) = default;
        MaterialComponent(Ref<Material> material)
            : material(material) {}

        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::make_nvp("Material", *material));
        }
    };

    struct LightComponent //Do it well, this was only to experiment
    {
        enum Type
        {
            DirectionalLight = 0,
            PointLight = 1,
            SpotLight = 2
        };

        alignas(16) glm::vec3 Color = {150.0f, 150.0f, 150.0f};
        alignas(16) glm::vec3 Direction = {0.0f, -1.0f, 0.0f};
        alignas(16) glm::vec3 Position = {0.0f, 0.0f, 0.0f};

        float Range = 5.0f;
        float Attenuation = 1.0f;
        float Intensity = 1.0f;

        float Angle = 45.0f;

        int type = static_cast<int>(Type::DirectionalLight);

        LightComponent() = default;
        LightComponent(const LightComponent&) = default;

        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::make_nvp("Color", Color), cereal::make_nvp("Direction", Direction), cereal::make_nvp("Position", Position), cereal::make_nvp("Range", Range), cereal::make_nvp("Attenuation", Attenuation), cereal::make_nvp("Intensity", Intensity), cereal::make_nvp("Angle", Angle), cereal::make_nvp("Type", type));
        }
    };
}
