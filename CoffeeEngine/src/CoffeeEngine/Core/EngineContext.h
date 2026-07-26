#pragma once

namespace Coffee
{
    class Window;
    class Renderer;
    class RendererAPI;
    class Input;
    class ResourceManager;
    class SceneManager;
    class ProjectManager;
    class ScriptingManager;
    class Audio;

    struct EngineContext
    {
        Window* window; ///< Pointer to the main engine window.
        Renderer* renderer; ///< Pointer to the renderer used by the engine.
        RendererAPI* rendererAPI; ///< Pointer to the renderer API used by the engine.
        Input* input; ///< Pointer to the input system used by the engine.
        ResourceManager* resourceManager; ///< Pointer to the resource manager used by the engine.
        SceneManager* sceneManager; ///< Pointer to the scene manager used by the engine.
        ProjectManager* projectManager; ///< Pointer to the project manager used by the engine.
        ScriptingManager* scripting; ///< Pointer to the scripting manager used by the engine.
        Audio* audio; ///< Pointer to the audio system used by the engine.
    };
}