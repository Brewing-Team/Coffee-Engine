#pragma once

#include <glm/glm.hpp>
#include <cereal/access.hpp>

namespace Coffee {

    /**
     * @brief Structure representing an axis-aligned bounding box (AABB).
     */
    struct AABB {

        glm::vec3 min = glm::vec3(0.0f); ///< The minimum point of the AABB.
        glm::vec3 max = glm::vec3(0.0f); ///< The maximum point of the AABB.

        AABB() = default;

        /**
         * @brief Constructs an AABB with specified minimum and maximum points.
         * @param min The minimum point of the AABB.
         * @param max The maximum point of the AABB.
         */
        AABB(const glm::vec3& min, const glm::vec3& max)
            : min(min), max(max) {}

        private:
            friend class cereal::access;

            template<class Archive>
            void serialize(Archive& archive)
            {
                archive(min, max);
            }
    };

    /**
     * @brief Structure representing an oriented bounding box (OBB).
     */
    struct OBB
    {
        std::array<glm::vec3, 8> corners; ///< The corners of the OBB.

        OBB() = default;

        /**
         * @brief Constructs an OBB with specified corners.
         * @param corners The corners of the OBB.
         */
        OBB(const std::array<glm::vec3, 8>& corners)
            : corners(corners) {}

        /**
         * @brief Constructs an OBB from a transformation matrix and an AABB.
         * @param transform The transformation matrix.
         * @param aabb The axis-aligned bounding box.
         */
        OBB(const glm::mat4& transform, const AABB& aabb)
            : corners({
                glm::vec3(transform * glm::vec4(aabb.min.x, aabb.min.y, aabb.min.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.max.x, aabb.min.y, aabb.min.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.max.x, aabb.max.y, aabb.min.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.min.x, aabb.max.y, aabb.min.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.min.x, aabb.min.y, aabb.max.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.max.x, aabb.min.y, aabb.max.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.max.x, aabb.max.y, aabb.max.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.min.x, aabb.max.y, aabb.max.z, 1.0f))
                }) {}
    };

}