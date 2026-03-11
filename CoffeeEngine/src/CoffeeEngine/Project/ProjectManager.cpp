#include "ProjectManager.h"
#include "Project.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/CacheManager.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"
#include "CoffeeEngine/IO/ResourceLoader.h"

#include <fstream>

namespace Coffee {

    Ref<Project> ProjectManager::NewProject(const std::filesystem::path& path)
    {
        m_ActiveProject = CreateRef<Project>(path.filename().string(), path.parent_path(), ".CoffeeEngine/Cache/");

        CacheManager::SetCachePath(m_ActiveProject->m_ProjectDirectory / m_ActiveProject->m_CacheDirectory);
        ResourceLoader::SetWorkingDirectory(m_ActiveProject->m_ProjectDirectory);
        SceneManager::SetWorkingDirectory(m_ActiveProject->m_ProjectDirectory);

        return m_ActiveProject;
    }

    Ref<Project> ProjectManager::LoadProject(const std::filesystem::path& path)
    {
        Ref<Project> project = CreateRef<Project>();

        std::ifstream projectFile(path);
        cereal::JSONInputArchive archive(projectFile);

        archive(*project);

        project->m_ProjectDirectory = path.parent_path();

        m_ActiveProject = project;

        ResourceRegistry::Clear();

        CacheManager::SetCachePath(project->m_ProjectDirectory / project->m_CacheDirectory);
        ResourceLoader::SetWorkingDirectory(m_ActiveProject->m_ProjectDirectory);
        ResourceLoader::LoadDirectory(project->m_ProjectDirectory);
        SceneManager::SetWorkingDirectory(m_ActiveProject->m_ProjectDirectory);
        ScriptingManager::SetWorkingDirectory(m_ActiveProject->m_ProjectDirectory);
        Input::Load();
        Audio::OnProjectLoad();

        return project;
    }

    void ProjectManager::SaveActive()
    {
        if (m_ActiveProject)
        {
            std::filesystem::path path = m_ActiveProject->m_ProjectDirectory / m_ActiveProject->m_Name;

            std::ofstream projectFile(path);
            cereal::JSONOutputArchive archive(projectFile);

            archive(cereal::make_nvp("Project", *m_ActiveProject));

            Input::Save();
        }
    }

} // namespace Coffee