#include "SceneManager.h"
#include <filesystem>
#include <tracy/Tracy.hpp>
#include <future>

namespace Coffee {

    Ref<Scene> SceneManager::m_ActiveScene = nullptr;
    std::filesystem::path SceneManager::s_WorkingDirectory = std::filesystem::current_path();

    Ref<Scene> SceneManager::PreloadScene(const std::filesystem::path& scenePath)
    {
        ZoneScoped;

        auto scene = Scene::Load(scenePath);

        return scene;
    }

    std::future<Ref<Scene>> SceneManager::PreloadSceneAsync(const std::filesystem::path& scenePath)
    {
        return std::async(std::launch::async, [scenePath]() -> Ref<Scene> {

            ZoneScoped;

            Ref<Scene> scene = CreateRef<Scene>();
        
            std::ifstream sceneFile(scenePath);
            if (!sceneFile.is_open()) {
                std::cerr << "Failed to open scene file: " << scenePath << std::endl;
                return nullptr;
            }

            try {
                cereal::JSONInputArchive archive(sceneFile);
                archive(*scene);
                scene->SetFilePath(scenePath);
            } catch (const std::exception &e) {
                std::cerr << "Failed to load scene from file: " << scenePath << ". Error: " << e.what() << std::endl;
                return nullptr;
            }

            return scene;
        });
    }

    void SceneManager::ChangeScene(const std::filesystem::path& scenePath)
    {
        ZoneScoped;

        std::filesystem::path fullPath = s_WorkingDirectory / scenePath;

        m_ActiveScene = Scene::Load(scenePath);
    }

    void SceneManager::ChangeScene(const Ref<Scene>& scene)
    {
        ZoneScoped;

        m_ActiveScene = scene;
    }
}