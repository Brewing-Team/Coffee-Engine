#pragma once

#include "CoffeeEngine/Scene/Scene.h"
#include <filesystem>
#include <future>

namespace Coffee {

    class SceneManager
    {
    public:
        
        static Ref<Scene> PreloadScene(const std::filesystem::path& scenePath);
        static std::future<Ref<Scene>> PreloadSceneAsync(const std::filesystem::path& scenePath);

        static void ChangeScene(const std::filesystem::path& scenePath);
        static void ChangeScene(const Ref<Scene>& scene);
        /* static void ChangeSceneAsync(const std::filesystem::path& scenePath); */

        static Ref<Scene>& GetActiveScene() { return m_ActiveScene; }

        static void SetWorkingDirectory(const std::filesystem::path& workingDirectory) { s_WorkingDirectory = workingDirectory; }
    private:
        static std::filesystem::path s_WorkingDirectory;
        static Ref<Scene> m_ActiveScene;
    };

}