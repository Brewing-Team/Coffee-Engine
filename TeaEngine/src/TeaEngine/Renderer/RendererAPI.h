#pragma once

#include "TeaEngine/Renderer/VertexArray.h"

namespace Tea {

    class RendererAPI{
    public:

        void DrawIndexed(const Ref<VertexArray>& vertexArray);

        static Scope<RendererAPI> Create();
    };

};