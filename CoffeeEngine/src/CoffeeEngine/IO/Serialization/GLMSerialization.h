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

/** @} */
