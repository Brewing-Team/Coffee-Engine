#pragma once

#include "CoffeeEngine/Scene/Scene.h"
#include <filesystem>

namespace Coffee {

    class SceneManager
    {
    public:
        
        static Ref<Scene> LoadScene(const std::filesystem::path& scenePath);
        static Ref<Scene> LoadSceneAsync(const std::filesystem::path& scenePath);

        static void ChangeScene(const Ref<Scene>& scene);
        static void ChangeScene(const std::filesystem::path& scenePath);
/*         static void ChangeSceneAsync(const Ref<Scene>& scene);
        static void ChangeSceneAsync(const std::filesystem::path& scenePath); */

        static Ref<Scene>& GetActiveScene();

        static void SetWorkingDirectory(const std::filesystem::path& workingDirectory);
    private:
        static std::filesystem::path s_WorkingDirectory;
        static Ref<Scene> m_ActiveScene;
    };

}