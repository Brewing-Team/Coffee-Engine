#pragma once

#include <cereal/cereal.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

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
     * @brief Serializes a glm::ivec4 object.
     * @tparam Archive The type of the archive.
     * @param archive The archive to serialize to.
     * @param vec The glm::ivec4 object to serialize.
     */
    template<class Archive>
    void serialize(Archive& archive, glm::ivec4& vec)
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
    
    template<class Archive>
    void serialize(Archive& archive, glm::mat4& mat)
    {
        archive(cereal::make_nvp("m00", mat[0][0]), cereal::make_nvp("m01", mat[0][1]), cereal::make_nvp("m02", mat[0][2]), cereal::make_nvp("m03", mat[0][3]),
                cereal::make_nvp("m10", mat[1][0]), cereal::make_nvp("m11", mat[1][1]), cereal::make_nvp("m12", mat[1][2]), cereal::make_nvp("m13", mat[1][3]),
                cereal::make_nvp("m20", mat[2][0]), cereal::make_nvp("m21", mat[2][1]), cereal::make_nvp("m22", mat[2][2]), cereal::make_nvp("m23", mat[2][3]),
                cereal::make_nvp("m30", mat[3][0]), cereal::make_nvp("m31", mat[3][1]), cereal::make_nvp("m32", mat[3][2]), cereal::make_nvp("m33", mat[3][3]));
    }
}

/** @} */
