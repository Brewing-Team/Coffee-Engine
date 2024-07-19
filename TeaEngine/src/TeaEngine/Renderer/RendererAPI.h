#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Tea {

    class RendererAPI{
    public:

        static void Init();

        static void SetClearColor(const glm::vec4& color);
        static void Clear();

        static void DrawIndexed(const Ref<VertexArray>& vertexArray);

        static Scope<RendererAPI> Create();

        private:
        static Scope<RendererAPI> s_RendererAPI;
    };

};