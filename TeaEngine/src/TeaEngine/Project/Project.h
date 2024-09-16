#pragma once

#include "TeaEngine/Core/Base.h"
#include <cereal/cereal.hpp>
#include <filesystem>
#include <string>

namespace Tea {

    /**
     * @defgroup project Project
     * @brief Project management components of the TeaEngine.
     * @{
     */

    /**
     * @brief The Project class is responsible for managing project data such as name, directory, and start scene path.
     */
    class Project {
    public:
        /**
         * @brief Creates a new project.
         * @return A reference to the newly created project.
         */
        static Ref<Project> New();

        /**
         * @brief Loads a project from the specified path.
         * @param path The path to the project file.
         * @return A reference to the loaded project.
         */
        static Ref<Project> Load(const std::filesystem::path& path);

        /**
         * @brief Saves the active project to the specified path.
         * @param path The path to save the project file.
         */
        static void SaveActive(const std::filesystem::path& path);

        /**
         * @brief Gets the active project.
         * @return A reference to the active project.
         */
        static Ref<Project> GetActive() { return s_ActiveProject; }

        /**
         * @brief Gets the directory of the active project.
         * @return The path to the project directory.
         */
        static const std::filesystem::path& GetProjectDirectory() { return s_ActiveProject->m_ProjectDirectory; }

        /**
         * @brief Gets the name of the active project.
         * @return The name of the project.
         */
        static const std::string& GetProjectName() { return s_ActiveProject->m_Name; }

        /**
         * @brief Serializes the project data.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template<class Archive>
        void serialize(Archive& archive) { archive(cereal::make_nvp("Name", m_Name), cereal::make_nvp("StartScene", m_StartScenePath.string())); }

    private:
        std::string m_Name = "Untitled"; ///< The name of the project.
        std::filesystem::path m_ProjectDirectory; ///< The directory of the project.

        std::filesystem::path m_StartScenePath; ///< The path to the start scene.

        inline static Ref<Project> s_ActiveProject; ///< The active project.
    };

    /** @} */
}