#pragma once

#include <cereal/cereal.hpp>
#include <filesystem>

#include "CoffeeEngine/Core/Base.h"

namespace Coffee {

    /**
     * @defgroup project Project
     * @brief Project management components of the CoffeeEngine.
     * @{
     */

    /**
     * @brief The Project class is responsible for managing project data such as name, directory, and start scene path.
     */
    class Project {
    public:

        Project(const std::string& name, const std::filesystem::path& projectDirectory, const std::filesystem::path& cacheDirectory)
            : m_Name(name), m_ProjectDirectory(projectDirectory), m_CacheDirectory(cacheDirectory) {}

        /**
         * @brief Gets the directory of the project.
         * @return The path to the project directory.
         */
        const std::filesystem::path& GetDirectory() const { return m_ProjectDirectory; }

        /**
         * @brief Gets the name of the project.
         * @return The name of the project.
         */
        const std::string& GetName() const { return m_Name; }

        /**
         * @brief Retrieves the cache directory path of the project.
         * 
         * This method returns a constant reference to the cache directory path
         * associated with this project.
         * 
         * @return std::filesystem::path Reference to the cache directory path.
         */
        std::filesystem::path GetCacheDirectory() { return GetDirectory() / m_CacheDirectory; }

        /**
         * @brief Retrieves de audio directory path of the project
         *
         * This static method returns a reference to the audio directory absolute path associated with the
         * project
         * If no audio directory has been defined, it returns the project's directory path instead
         *
         * @return audio directory absolute path
         */
        std::filesystem::path GetAudioDirectory() { return GetDirectory() / GetRelativeAudioDirectory(); }

        /**
         * @brief Retrieves the audio directory relative path of the active object
         *
         * This static method returns a reference to the audio directory relative path associated with the currently
         * active project. If no audio directory has been defined, it returns an empty path instead
         *
         * @return The audio directory relative path
         */
        std::filesystem::path GetRelativeAudioDirectory() { return m_AudioFolderPath; }

        /**
         * @brief Serializes the project data.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template<class Archive> void serialize(Archive& archive, std::uint32_t const version);

    private:
        std::string m_Name = "Untitled"; ///< The name of the project.
        std::filesystem::path m_ProjectDirectory; ///< The directory of the project.
        std::filesystem::path m_CacheDirectory; ///< The directory of the project cache.

        std::filesystem::path m_StartScenePath; ///< The path to the start scene.
        std::filesystem::path m_AudioFolderPath; ///< The path to the audio folder

        // INFO: This is for making the Project read-only outside of the ProjectManager, which is responsible for managing project creation, loading, and saving operations.
        friend class ProjectManager; ///< Granting access to private members for ProjectManager.
    };

    /** @} */
}
CEREAL_CLASS_VERSION(Coffee::Project, 1)