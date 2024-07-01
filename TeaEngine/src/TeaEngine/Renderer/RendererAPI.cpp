#include "TeaEngine/Renderer/RendererAPI.h"

#include <glad/glad.h>

namespace Tea {

    void RendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        uint32_t count = vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    Scope<RendererAPI> RendererAPI::Create()
    {
        return CreateScope<RendererAPI>();
    }

}