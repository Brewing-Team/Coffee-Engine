#include "ProjectManager.h"
#include "Project.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Resources/CacheManager.h"
#include "CoffeeEngine/Resources/ResourceRegistry.h"
#include "CoffeeEngine/Resources/ResourceManager.h"

#include <fstream>

namespace Coffee {

        ProjectManager::ProjectManager(SceneManager* sceneManager, ResourceManager* resourceManager, ScriptingManager* scriptingManager, Audio* audio)
            : m_SceneManager(sceneManager), m_ResourceManager(resourceManager), m_ScriptingManager(scriptingManager), m_Audio(audio)
        {
        }

    Ref<const Project> ProjectManager::NewProject(const std::filesystem::path& path)
    {
        m_CurrentProject = CreateRef<Project>(path.filename().string(), path.parent_path(), ".CoffeeEngine/Cache/");
        
        // TODO: Move this logic to and event that is fired when a project is loaded or created. It should be handled by the ResourceManager or something like that.
        CacheManager::SetCachePath(m_CurrentProject->m_ProjectDirectory / m_CurrentProject->m_CacheDirectory);
        m_ResourceManager->SetWorkingDirectory(m_CurrentProject->m_ProjectDirectory);
        m_SceneManager->SetWorkingDirectory(m_CurrentProject->m_ProjectDirectory);

        return m_CurrentProject;
    }

    Ref<const Project> ProjectManager::LoadProject(const std::filesystem::path& path)
    {
        Ref<Project> project = CreateRef<Project>();

        std::ifstream projectFile(path);
        cereal::JSONInputArchive archive(projectFile);

        archive(*project);

        project->m_ProjectDirectory = path.parent_path();

        m_CurrentProject = project;

        // TODO: Move the next logic to and event that is fired when a project is loaded or created. It should be handled by the ResourceManager or something like that.

        m_ResourceManager->ClearRegistry();

        CacheManager::SetCachePath(project->m_ProjectDirectory / project->m_CacheDirectory);
        m_ResourceManager->SetWorkingDirectory(m_CurrentProject->m_ProjectDirectory);
        m_ResourceManager->LoadDirectory(project->m_ProjectDirectory);
        m_SceneManager->SetWorkingDirectory(m_CurrentProject->m_ProjectDirectory);
        m_ScriptingManager->SetWorkingDirectory(m_CurrentProject->m_ProjectDirectory);
        m_Audio->OnProjectLoad();

        return project;
    }

    void ProjectManager::SaveCurrentProject()
    {
        if (m_CurrentProject)
        {
            std::filesystem::path path = m_CurrentProject->m_ProjectDirectory / m_CurrentProject->m_Name;

            std::ofstream projectFile(path);
            cereal::JSONOutputArchive archive(projectFile);

            archive(cereal::make_nvp("Project", *m_CurrentProject));
        }
    }

} // namespace Coffee