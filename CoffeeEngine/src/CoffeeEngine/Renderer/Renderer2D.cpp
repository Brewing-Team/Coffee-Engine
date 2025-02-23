#include "Renderer2D.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/VertexArray.h"
#include "CoffeeEngine/Renderer/RendererAPI.h"

#include "CoffeeEngine/Embedded/QuadShader.inl"

#include <array>
#include <cstddef>
#include <glm/glm.hpp>

#include <cstdint>
#include <vector>

namespace Coffee {

    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        
        float TexIndex;
        float TilingFactor;

        uint32_t EntityID;
    };

    struct LineVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        
        uint32_t EntityID;
    };

    struct TextVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;

        uint32_t EntityID;
    };

    struct Batch
    {
        static const uint32_t MaxQuadCount = 10000; // Think of increasing this number to 20000
        static const uint32_t MaxVertices = MaxQuadCount * 4;
        static const uint32_t MaxIndices = MaxQuadCount * 6;

        std::vector<QuadVertex> QuadVertices;
        uint32_t QuadIndexCount = 0;

        std::vector<LineVertex> LineVertices;
        uint32_t LineIndexCount = 0;

        std::vector<TextVertex> TextVertices;
        uint32_t TextIndexCount = 0;
    };

    struct Renderer2DData
    {
        std::vector<Batch> Batches;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;

        Ref<VertexArray> LineVertexArray;
        Ref<VertexBuffer> LineVertexBuffer;

        Ref<VertexArray> TextVertexArray;
        Ref<VertexBuffer> TextVertexBuffer;

        Ref<Shader> QuadShader;

        glm::vec4 QuadVertexPositions[4] = {
            {-0.5f, -0.5f, 0.0f, 1.0f},
            {0.5f, -0.5f, 0.0f, 1.0f},
            {0.5f, 0.5f, 0.0f, 1.0f},
            {-0.5f, 0.5f, 0.0f, 1.0f}
        };

    };

    static Renderer2DData s_Renderer2DData;

    void Renderer2D::Init()
    {
        BufferLayout quadLayout = {
            {ShaderDataType::Vec3, "a_Position"},
            {ShaderDataType::Vec4, "a_Color"},
            {ShaderDataType::Vec2, "a_TexCoord"},
            {ShaderDataType::Float, "a_TexIndex"},
            {ShaderDataType::Float, "a_TilingFactor"},
            {ShaderDataType::Int, "a_EntityID"}
        };

        s_Renderer2DData.QuadVertexArray = VertexArray::Create();
        s_Renderer2DData.QuadVertexBuffer = VertexBuffer::Create(Batch::MaxVertices * sizeof(QuadVertex));
        s_Renderer2DData.QuadVertexBuffer->SetLayout(quadLayout);
        s_Renderer2DData.QuadVertexArray->AddVertexBuffer(s_Renderer2DData.QuadVertexBuffer);

        s_Renderer2DData.QuadShader = CreateRef<Shader>("QuadShader", std::string(quadShaderSource));
    }

    void Renderer2D::Render(const RenderTarget& target)
    {
        // TODO: Modify the target to have a framebuffer for 2D elements
        const Ref<Framebuffer>& forwardBuffer = target.GetFramebuffer("Forward");

        forwardBuffer->Bind();
        //forwardBuffer->SetDrawBuffers({0, 1}); //TODO: This should only be done in the editor

        for (Batch& batch : s_Renderer2DData.Batches)
        {
            if(batch.QuadIndexCount > 0)
            {
                s_Renderer2DData.QuadVertexBuffer->SetData(batch.QuadVertices.data(), batch.QuadVertices.size() * sizeof(QuadVertex));
                s_Renderer2DData.QuadShader->Bind();
                RendererAPI::DrawIndexed(s_Renderer2DData.QuadVertexArray, batch.QuadIndexCount);
            }
        }

        forwardBuffer->UnBind();

        s_Renderer2DData.Batches.clear();
    }

    void Renderer2D::Shutdown()
    {
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, uint32_t entityID)
    {
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 texCoords[] = {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f}
        };

        // TODO: Think if this should be done here
        if(s_Renderer2DData.Batches.empty())
        {
            s_Renderer2DData.Batches.push_back(Batch());
        }

        Batch& batch = s_Renderer2DData.Batches.back();

        if(batch.QuadIndexCount >= Batch::MaxIndices)
        {
            // TODO: Wrap this in a function

            s_Renderer2DData.Batches.push_back(Batch());
            batch = s_Renderer2DData.Batches.back();
        }

        for(size_t i = 0; i < quadVertexCount; i++)
        {
            batch.QuadVertices.push_back(
            {
                transform * s_Renderer2DData.QuadVertexPositions[i], 
                color, 
                texCoords[i], 
                0.0f, 
                1.0f, 
                entityID
                });
        }

        batch.QuadIndexCount += 6;
    }

}