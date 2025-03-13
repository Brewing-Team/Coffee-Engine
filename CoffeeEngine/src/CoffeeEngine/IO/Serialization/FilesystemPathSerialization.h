/**
 * @defgroup io IO
 * @brief IO components of the CoffeeEngine.
 * @{
 */

#pragma once

#include <cereal/cereal.hpp>
#include <filesystem>

namespace cereal {
    /**
     * @brief Serializes a std::filesystem::path object.
     *
     * @tparam Archive The type of the archive.
     * @param archive The archive to save the path to.
     * @param path The filesystem path to be serialized.
     */
    template <class Archive>
    void save(Archive& archive, const std::filesystem::path& path)
    {
        archive(path.generic_string());
    }

    /**
     * @brief Deserializes a std::filesystem::path object.
     *
     * @tparam Archive The type of the archive.
     * @param archive The archive to load the path from.
     * @param path The filesystem path to be deserialized.
     */
    template <class Archive>
    void load(Archive& archive, std::filesystem::path& path)
    {
        std::string pathStr; ///< The string representation of the path.
        archive(pathStr); ///< Load the string representation of the path.
        path = std::filesystem::path(pathStr); ///< Convert the string to a filesystem path.
    }
}

/** @} */