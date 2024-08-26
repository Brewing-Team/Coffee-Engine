#include "Project.h"
#include "TeaEngine/Core/Base.h"

#include <cereal/archives/json.hpp>

#include <fstream>

namespace Tea {

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

        return project;
    }

    void Project::SaveActive(const std::filesystem::path& path)
    {
        std::ofstream projectFile(path);
        cereal::JSONOutputArchive archive(projectFile);

        archive(*s_ActiveProject);
        s_ActiveProject->m_ProjectDirectory = path;
    }

}