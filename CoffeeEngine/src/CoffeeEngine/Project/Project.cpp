#include "Project.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"

#include <cereal/archives/json.hpp>

#include <fstream>

namespace Coffee {

    static Ref<Project> s_ActiveProject;

    Ref<Project> Project::New()
    {
        s_ActiveProject = CreateRef<Project>();
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

        ResourceLoader::LoadResources(project->m_ProjectDirectory);

        return project;
    }

    void Project::SaveActive(const std::filesystem::path& path)
    {
        s_ActiveProject->m_ProjectDirectory = path.parent_path();
        s_ActiveProject->m_Name = path.stem().string();

        std::ofstream projectFile(path);
        cereal::JSONOutputArchive archive(projectFile);

        archive(cereal::make_nvp("Project", *s_ActiveProject));
    }

}