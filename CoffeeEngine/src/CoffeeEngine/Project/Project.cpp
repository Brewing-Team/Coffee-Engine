#include "Project.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/ResourceImporter.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"

#include <cereal/archives/json.hpp>

#include <fstream>

namespace Coffee {

    static Ref<Project> s_ActiveProject;

    Ref<Project> Project::New(const std::filesystem::path& path)
    {
        s_ActiveProject = CreateRef<Project>();

        s_ActiveProject->m_ProjectDirectory = path.parent_path();
        s_ActiveProject->m_Name = path.filename().string();
        if(s_ActiveProject->m_CacheDirectory.empty())
        {
            s_ActiveProject->m_CacheDirectory = ".CoffeeEngine/Cache/";
        }

        ResourceImporter::SetCachePath((path.parent_path() / s_ActiveProject->m_CacheDirectory / "Resources"));

        return s_ActiveProject;
    }

    Ref<Project> Project::Load(const std::filesystem::path& path)
    {
        Ref<Project> project = CreateRef<Project>();

        std::ifstream projectFile(path);
        cereal::JSONInputArchive archive(projectFile);

        archive(*project);

        project->m_ProjectDirectory = path.parent_path();

        s_ActiveProject = project;

        ResourceRegistry::Clear();

        ResourceImporter::SetCachePath((path.parent_path() / s_ActiveProject->m_CacheDirectory / "Resources"));
        ResourceLoader::LoadResources(project->m_ProjectDirectory);

        return project;
    }

    void Project::SaveActive()
    {
        std::filesystem::path path = s_ActiveProject->m_ProjectDirectory / s_ActiveProject->m_Name;

        std::ofstream projectFile(path);
        cereal::JSONOutputArchive archive(projectFile);

        archive(cereal::make_nvp("Project", *s_ActiveProject));
    }

}