/**
 * @defgroup scene Scene
 * @{
 */

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

// TEMPORAL: Move this to the serialization folder!!
namespace cereal {
    /**
     * @brief Serializes a glm::vec2 object.
     * @tparam Archive The type of the archive.
     * @param archive The archive to serialize to.
     * @param vec The glm::vec2 object to serialize.
     */
    template<class Archive>
    void serialize(Archive& archive, glm::vec2& vec)
    {
        archive(cereal::make_nvp("x", vec.x), cereal::make_nvp("y", vec.y));
    }

    /**
     * @brief Serializes a glm::vec3 object.
     * @tparam Archive The type of the archive.
     * @param archive The archive to serialize to.
     * @param vec The glm::vec3 object to serialize.
     */
    template<class Archive>
    void serialize(Archive& archive, glm::vec3& vec)
    {
        archive(cereal::make_nvp("x", vec.x), cereal::make_nvp("y", vec.y), cereal::make_nvp("z", vec.z));
    }

    /**
     * @brief Serializes a glm::vec4 object.
     * @tparam Archive The type of the archive.
     * @param archive The archive to serialize to.
     * @param vec The glm::vec4 object to serialize.
     */
    template<class Archive>
    void serialize(Archive& archive, glm::vec4& vec)
    {
        archive(cereal::make_nvp("x", vec.x), cereal::make_nvp("y", vec.y), cereal::make_nvp("z", vec.z), cereal::make_nvp("w", vec.w));
    }

    /**
     * @brief Serializes a glm::quat object.
     * @tparam Archive The type of the archive.
     * @param archive The archive to serialize to.
     * @param quat The glm::quat object to serialize.
     */
    template<class Archive>
    void serialize(Archive& archive, glm::quat& quat)
    {
        archive(cereal::make_nvp("x", quat.x), cereal::make_nvp("y", quat.y), cereal::make_nvp("z", quat.z), cereal::make_nvp("w", quat.w));
    }
}

namespace Coffee {
    /**
     * @brief Component representing a tag.
     * @ingroup scene
     */
    struct TagComponent
    {
        std::string Tag; ///< The tag string.

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag)
            : Tag(tag) {}

        /**
         * @brief Serializes the TagComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::make_nvp("Tag", Tag));
        }
    };

    /**
     * @brief Component representing a transform.
     * @ingroup scene
     */
    struct TransformComponent
    {
    private:
        glm::mat4 worldMatrix = glm::mat4(1.0f); ///< The world transformation matrix.
    public:
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f }; ///< The position vector.
        glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f }; ///< The rotation vector.
        glm::vec3 Scale = { 1.0f, 1.0f, 1.0f }; ///< The scale vector.

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& position)
            : Position(position) {}

        /**
         * @brief Gets the local transformation matrix.
         * @return The local transformation matrix.
         */
        glm::mat4 GetLocalTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));

            return glm::translate(glm::mat4(1.0f), Position)
                    * rotation
                    * glm::scale(glm::mat4(1.0f), Scale);
        }

        /**
         * @brief Sets the local transformation matrix.
         * @param transform The transformation matrix to set.
         */
        void SetLocalTransform(const glm::mat4& transform) //TODO: Improve this function, this way is ugly and glm::decompose is from gtx (is supposed to not be very stable)
        {
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::quat orientation;

            glm::decompose(transform, Scale, orientation, Position, skew, perspective);
            Rotation = glm::degrees(glm::eulerAngles(orientation));
        }

        /**
         * @brief Gets the world transformation matrix.
         * @return The world transformation matrix.
         */
        const glm::mat4& GetWorldTransform() const
        {
            return worldMatrix;
        }

        /**
         * @brief Sets the world transformation matrix.
         * @param transform The transformation matrix to set.
         */
        void SetWorldTransform(const glm::mat4& transform)
        {
            worldMatrix = transform * GetLocalTransform();
        }

        /**
         * @brief Serializes the TransformComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::make_nvp("Position", Position), cereal::make_nvp("Rotation", Rotation), cereal::make_nvp("Scale", Scale));
        }
    };

    /**
     * @brief Component representing a camera.
     * @ingroup scene
     */
    struct CameraComponent
    {
        SceneCamera Camera; ///< The scene camera.

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        /**
         * @brief Serializes the CameraComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::make_nvp("Camera", Camera));
        }
    };

    /**
     * @brief Component representing a mesh.
     * @ingroup scene
     */
    struct MeshComponent
    {
        Ref<Mesh> mesh; ///< The mesh reference.
        bool drawAABB = false; ///< Flag to draw the axis-aligned bounding box (AABB).

        MeshComponent()
        {
            // TEMPORAL! In the future use for example MeshComponent() : mesh(MeshFactory(PrimitiveType::MeshText))
            Ref<Model> m = Model::Load("assets/models/MissingMesh.glb");
            mesh = m->GetMeshes()[0];
        }
        MeshComponent(const MeshComponent&) = default;
        MeshComponent(Ref<Mesh> mesh)
            : mesh(mesh) {}

        /**
         * @brief Gets the mesh reference.
         * @return The mesh reference.
         */
        const Ref<Mesh>& GetMesh() const { return mesh; }

        /**
         * @brief Serializes the MeshComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template<class Archive>
        void save(Archive& archive)
        {
            archive(cereal::make_nvp("Mesh", *mesh));
        }

        /**
         * @brief Deserializes the MeshComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to deserialize from.
         */
        template<class Archive>
        void load(Archive& archive)
        {
            archive(*mesh);
        }
    };

    /**
     * @brief Component representing a material.
     * @ingroup scene
     */
    struct MaterialComponent
    {
        Ref<Material> material; ///< The material reference.

        MaterialComponent()
            : material(CreateRef<Material>()) {}
        MaterialComponent(const MaterialComponent&) = default;
        MaterialComponent(Ref<Material> material)
            : material(material) {}

        /**
         * @brief Serializes the MaterialComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::make_nvp("Material", *material));
        }
    };

    /**
     * @brief Component representing a light.
     * @ingroup scene
     */
    struct LightComponent
    {
        /**
         * @brief Enum representing the type of light.
         */
        enum Type
        {
            DirectionalLight = 0, ///< Directional light.
            PointLight = 1, ///< Point light.
            SpotLight = 2 ///< Spot light.
        };

        // Align to 16 bytes(glm::vec4) instead of 12 bytes(glm::vec3) to match the std140 layout in the shader (a vec3 is 16 bytes in std140)
        alignas(16) glm::vec3 Color = {1.0f, 1.0f, 1.0f}; ///< The color of the light.
        alignas(16) glm::vec3 Direction = {0.0f, -1.0f, 0.0f}; ///< The direction of the light.
        alignas(16) glm::vec3 Position = {0.0f, 0.0f, 0.0f}; ///< The position of the light.

        float Range = 5.0f; ///< The range of the light.
        float Attenuation = 1.0f; ///< The attenuation of the light.
        float Intensity = 1.0f; ///< The intensity of the light.

        float Angle = 45.0f; ///< The angle of the light.

        int type = static_cast<int>(Type::DirectionalLight); ///< The type of the light.

        LightComponent() = default;
        LightComponent(const LightComponent&) = default;

        /**
         * @brief Serializes the LightComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::make_nvp("Color", Color), cereal::make_nvp("Direction", Direction), cereal::make_nvp("Position", Position), cereal::make_nvp("Range", Range), cereal::make_nvp("Attenuation", Attenuation), cereal::make_nvp("Intensity", Intensity), cereal::make_nvp("Angle", Angle), cereal::make_nvp("Type", type));
        }
    };
}

/** @} */