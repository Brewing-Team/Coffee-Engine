#pragma once

#include <filesystem>

namespace Coffee {

    class Project; // Forward declaration

    /**
     * @defgroup project Project
     * @brief Project management components of the CoffeeEngine.
     * @{
     */

    /**
     * @brief Manages project creation, loading, and saving operations.
     */
    class ProjectManager {
    public:
        /**
         * @brief Creates a new project at the specified path.
         * @param path The directory path where the project will be created.
         * @return A reference to the newly created project.
         */
        Ref<const Project> NewProject(const std::filesystem::path& path);

        /**
         * @brief Loads a project from the specified path.
         * @param path The path to the project file.
         * @return A reference to the loaded project.
         */
        Ref<const Project> LoadProject(const std::filesystem::path& path);

        /**
         * @brief Saves the currently active project.
         */
        void SaveActive();

        /**
         * @brief Retrieves the currently active project.
         * @return A reference to the active project.
         */
        Ref<const Project> GetActive() { return m_ActiveProject; }

    private:
        Ref<const Project> m_ActiveProject; ///< The currently active project.
    };

    /** @} */
}