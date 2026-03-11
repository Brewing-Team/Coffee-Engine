#pragma once

namespace Coffee
{
    struct EngineContext
    {
        Renderer* renderer; ///< Pointer to the renderer used by the engine.
        RendererAPI* rendererAPI; ///< Pointer to the renderer API used by the engine.
        Input* input; ///< Pointer to the input system used by the engine.
        ResourceManager* resources; ///< Pointer to the resource manager used by the engine.
        SceneManager* sceneManager; ///< Pointer to the scene manager used by the engine.
        ProjectManager* projectManager; ///< Pointer to the project manager used by the engine.
        Audio* audio; ///< Pointer to the audio system used by the engine.
    };
}