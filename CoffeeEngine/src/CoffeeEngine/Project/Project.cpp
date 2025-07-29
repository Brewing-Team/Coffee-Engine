#include "Project.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Core/Input.h"
#include "CoffeeEngine/IO/CacheManager.h"
#include "CoffeeEngine/IO/ResourceRegistry.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/Scene/SceneManager.h"
#include "CoffeeEngine/Scripting/ScriptManager.h"
#include "CoffeeEngine/Audio/Audio.h"

#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

#include <fstream>

namespace Coffee {

    // Project implementation
    template<class Archive>
    void Project::serialize(Archive& archive, std::uint32_t const version)
    {
        archive(cereal::make_nvp("Name", m_Name),
                cereal::make_nvp("StartScene",m_StartScenePath.string()),
                cereal::make_nvp("CacheDirectory", m_CacheDirectory));

        if (version >= 1)
        {
            archive(cereal::make_nvp("AudioDirectory", m_AudioFolderPath));
        }
        else
        {
            m_AudioFolderPath = "";
        }
    }

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

        CacheManager::SetCachePath(s_ActiveProject->m_ProjectDirectory / s_ActiveProject->m_CacheDirectory);
        ResourceLoader::SetWorkingDirectory(s_ActiveProject->m_ProjectDirectory);
        SceneManager::SetWorkingDirectory(s_ActiveProject->m_ProjectDirectory);

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

        CacheManager::SetCachePath(project->m_ProjectDirectory / project->m_CacheDirectory);
        ResourceLoader::SetWorkingDirectory(s_ActiveProject->m_ProjectDirectory);
        ResourceLoader::LoadDirectory(project->m_ProjectDirectory);
        SceneManager::SetWorkingDirectory(s_ActiveProject->m_ProjectDirectory);
        ScriptManager::SetWorkingDirectory(s_ActiveProject->m_ProjectDirectory);
        Input::Load();
        Audio::OnProjectLoad();

        return project;
    }

    void Project::SaveActive()
    {
        if (s_ActiveProject)
        {
            std::filesystem::path path = s_ActiveProject->m_ProjectDirectory / s_ActiveProject->m_Name;

            std::ofstream projectFile(path);
            cereal::JSONOutputArchive archive(projectFile);

            archive(cereal::make_nvp("Project", *s_ActiveProject));

            Input::Save();
        }
    }
    // Explicit template instantiations for common cereal archives
    template void Project::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&, std::uint32_t const);
    template void Project::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&, std::uint32_t const);
    template void Project::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, std::uint32_t const);
    template void Project::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&, std::uint32_t const);

} // namespace Coffee