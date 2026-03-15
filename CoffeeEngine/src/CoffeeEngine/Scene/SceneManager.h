#pragma once

#include "CoffeeEngine/Scene/Scene.h"
#include <filesystem>
#include <future>

namespace Coffee {

    class SceneManager
    {
    public:
        enum class SceneState
        {
            Edit = 0,
            Play = 1
        };
    public:
        
        SceneManager();

        Ref<Scene> PreloadScene(const std::filesystem::path& scenePath);
        std::future<Ref<Scene>> PreloadSceneAsync(const std::filesystem::path& scenePath);

        void ChangeScene(const std::filesystem::path& scenePath);
        void ChangeScene(const Ref<Scene>& scene);
        void ChangeSceneAsync(const std::filesystem::path& scenePath);

        Ref<Scene>& GetActiveScene() { return m_ActiveScene; }

        void SetContext(const EngineContext& context) { m_Context = context; }

        void SetWorkingDirectory(const std::filesystem::path& workingDirectory) { m_WorkingDirectory = workingDirectory; }

        void SetSceneState(SceneState state) { m_SceneState = state; }
        SceneState GetSceneState() { return m_SceneState; }
        std::string GetSceneName() { return m_ActiveScene ? m_ActiveScene->GetFilePath().filename().string() : ""; }
    private:
        void ExitCurrentScene();
        void InitNewScene();
    private:
        SceneState m_SceneState = SceneState::Edit;
        EngineContext m_Context{};
        std::filesystem::path m_WorkingDirectory;
        Ref<Scene> m_ActiveScene;
    };

}