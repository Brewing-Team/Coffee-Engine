#pragma once

namespace Coffee
{
    struct EngineContext
    {
        Renderer* renderer;
        RendererAPI* rendererAPI;
        InputSystem* input;
        ResourceManager* resources;
    };
}