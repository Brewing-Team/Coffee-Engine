#pragma once

#include "TeaEngine/Core/Base.h"
#include <cereal/cereal.hpp>
#include <filesystem>
#include <string>
namespace Tea {

    class Project {
    public:
        static Ref<Project> New();
        static Ref<Project> Load(const std::filesystem::path& path);
        static void SaveActive(const std::filesystem::path& path);

        static Ref<Project> GetActive() { return s_ActiveProject; }
        static const std::filesystem::path& GetProjectDirectory() { return s_ActiveProject->m_ProjectDirectory; }

        template<class Archive>
        void serialize(Archive& archive) { archive(cereal::make_nvp("Name", m_Name), cereal::make_nvp("StartScene", m_StartScenePath.string())); }

    private:
        std::string m_Name = "Untitled";
        std::filesystem::path m_ProjectDirectory;

        std::filesystem::path m_StartScenePath;

        inline static Ref<Project> s_ActiveProject;
    };

}