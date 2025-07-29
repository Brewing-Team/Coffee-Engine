#pragma once

#include <cereal/cereal.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Coffee
{
    /**
     * @brief Component representing a transform.
     * @ingroup scene
     */
    struct TransformComponent
    {
      private:
        glm::vec3 Position = {0.0f, 0.0f, 0.0f}; ///< The position vector.
        glm::vec3 Rotation = {0.0f, 0.0f, 0.0f}; ///< The rotation vector.
        glm::vec3 Scale = {1.0f, 1.0f, 1.0f};    ///< The scale vector.

        glm::mat4 worldMatrix = glm::mat4(1.0f); ///< The world transformation matrix.
        bool isDirty = true;                     ///< Flag to indicate if the transform is dirty.
      public:
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& position) : Position(position) {}

        void SetLocalPosition(const glm::vec3& position)
        {
            Position = position;
            isDirty = true; // Mark the transform as dirty
        }

        void SetLocalRotation(const glm::vec3& rotation)
        {
            Rotation = rotation;
            isDirty = true; // Mark the transform as dirty
        }

        void SetLocalScale(const glm::vec3& scale)
        {
            Scale = scale;
            isDirty = true; // Mark the transform as dirty
        }

        /**
         * @brief Gets the local position vector.
         * @return The local position vector.
         */
        glm::vec3& GetLocalPosition() { return Position; }
        glm::vec3& GetLocalRotation() { return Rotation; }
        glm::vec3& GetLocalScale() { return Scale; }

        void SetWorldPosition(const glm::vec3& position)
        {
            Position = position;
            SetWorldTransform(glm::translate(glm::mat4(1.0f), Position) *
                              glm::toMat4(glm::quat(glm::radians(Rotation))) * glm::scale(glm::mat4(1.0f), Scale));
        }

        void SetWorldRotation(const glm::vec3& rotation)
        {
            Rotation = rotation;
            SetWorldTransform(glm::translate(glm::mat4(1.0f), Position) *
                              glm::toMat4(glm::quat(glm::radians(Rotation))) * glm::scale(glm::mat4(1.0f), Scale));
        }

        void SetWorldScale(const glm::vec3& scale)
        {
            Scale = scale;
            SetWorldTransform(glm::translate(glm::mat4(1.0f), Position) *
                              glm::toMat4(glm::quat(glm::radians(Rotation))) * glm::scale(glm::mat4(1.0f), Scale));
        }

        /**
         * @brief Gets the local transformation matrix.
         * @return The local transformation matrix.
         */
        glm::mat4 GetLocalTransform() const
        {
            glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(Rotation)));

            return glm::translate(glm::mat4(1.0f), Position) * rotation * glm::scale(glm::mat4(1.0f), Scale);
        }

        /**
         * @brief Sets the local transformation matrix.
         * @param transform The transformation matrix to set.
         */
        void SetLocalTransform(
            const glm::mat4& transform) // TODO: Improve this function, this way is ugly and glm::decompose is from gtx
                                        // (is supposed to not be very stable)
        {
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::quat orientation;

            glm::decompose(transform, Scale, orientation, Position, skew, perspective);
            Rotation = glm::degrees(glm::eulerAngles(orientation));
            isDirty = true; // Mark the transform as dirty
        }

        /**
         * @brief Gets the world transformation matrix.
         * @return The world transformation matrix.
         */
        const glm::mat4& GetWorldTransform() const { return worldMatrix; }

        /**
         * @brief Sets the world transformation matrix.
         * @param transform The transformation matrix to set.
         */
        void SetWorldTransform(const glm::mat4& transform)
        {
            worldMatrix = transform * GetLocalTransform();
            isDirty = false; // Mark the transform as clean
        }

        void MarkDirty()
        {
            isDirty = true; // Mark the transform as dirty
        }

        bool IsDirty() const
        {
            return isDirty; // Check if the transform is dirty
        }

        /**
         * @brief Serializes the TransformComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template <class Archive> void serialize(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::TransformComponent, 0);
