#pragma once

#include "TeaEngine/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Tea {

    class RendererAPI{
    public:

        void Init();

        void SetClearColor(const glm::vec4& color);
        void Clear();

        void DrawIndexed(const Ref<VertexArray>& vertexArray);

        static Scope<RendererAPI> Create();
    };

};