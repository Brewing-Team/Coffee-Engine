#pragma once

#include "CoffeeEngine/Scene/SceneManager.h"
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
        // INFO: Temporal dependencies, should be modified when we have a proper event system in place.
        ProjectManager(SceneManager* sceneManager, ResourceManager* resourceManager, ScriptingManager* scriptingManager, Audio* audio);

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
        void SaveCurrentProject();

        /**
         * @brief Retrieves the currently active project.
         * @return A reference to the active project.
         */
        Ref<const Project> GetCurrentProject() { return m_CurrentProject; }

    private:
        // TODO: Remove these dependencies and replace them with an event system or something like that.
        SceneManager* m_SceneManager;
        ResourceManager* m_ResourceManager;
        ScriptingManager* m_ScriptingManager;
        Audio* m_Audio;

        Ref<const Project> m_CurrentProject; ///< The currently active project.
    };

    /** @} */
}