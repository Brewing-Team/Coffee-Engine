#include "Renderer2D.h"
#include "CoffeeEngine/Renderer/MSDFData.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/Renderer/VertexArray.h"
#include "CoffeeEngine/Renderer/RendererAPI.h"
#include "CoffeeEngine/Scene/Scene.h"

#include "CoffeeEngine/Embedded/QuadShader.inl"
#include "CoffeeEngine/Embedded/TextShader.inl"

#include <array>
#include <cstddef>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

        glm::vec3 EntityID;
    };

    struct LineVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        
        glm::vec3 EntityID;
    };

    struct TextVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;

        glm::vec3 EntityID;
    };

    struct Batch
    {
        static const uint32_t MaxQuadCount = 10000; // Think of increasing this number to 20000
        static const uint32_t MaxVertices = MaxQuadCount * 4;
        static const uint32_t MaxIndices = MaxQuadCount * 6;
        static const uint32_t MaxTextureSlots = 32;

        std::vector<QuadVertex> QuadVertices;
        uint32_t QuadIndexCount = 0;

        std::vector<LineVertex> LineVertices;
        uint32_t LineIndexCount = 0;

        std::vector<TextVertex> TextVertices;
        uint32_t TextIndexCount = 0;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 is reserved for white texture

        Ref<Texture2D> FontAtlasTexture;
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
        Ref<Shader> LineShader;
        Ref<Shader> TextShader;

        Ref<Texture2D> WhiteTexture;

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
        s_Renderer2DData.QuadVertexArray = VertexArray::Create();

        s_Renderer2DData.QuadVertexBuffer = VertexBuffer::Create(Batch::MaxVertices * sizeof(QuadVertex));
        BufferLayout quadLayout = {
            {ShaderDataType::Vec3, "a_Position"},
            {ShaderDataType::Vec4, "a_Color"},
            {ShaderDataType::Vec2, "a_TexCoord"},
            {ShaderDataType::Float, "a_TexIndex"},
            {ShaderDataType::Float, "a_TilingFactor"},
            {ShaderDataType::Vec3, "a_EntityID"}
        };
        s_Renderer2DData.QuadVertexBuffer->SetLayout(quadLayout);
        s_Renderer2DData.QuadVertexArray->AddVertexBuffer(s_Renderer2DData.QuadVertexBuffer);

        std::vector<uint32_t> quadIndices(Batch::MaxIndices);

        uint32_t offset = 0;
        for(uint32_t i = 0; i < Batch::MaxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices.data(), quadIndices.size());
        s_Renderer2DData.QuadVertexArray->SetIndexBuffer(quadIB);
        quadIndices.clear();

        // Text
        s_Renderer2DData.TextVertexArray = VertexArray::Create();

        s_Renderer2DData.TextVertexBuffer = VertexBuffer::Create(Batch::MaxVertices * sizeof(TextVertex));
        BufferLayout textLayout = {
            {ShaderDataType::Vec3, "a_Position"},
            {ShaderDataType::Vec4, "a_Color"},
            {ShaderDataType::Vec2, "a_TexCoord"},
            {ShaderDataType::Vec3, "a_EntityID"}
        };
        s_Renderer2DData.TextVertexBuffer->SetLayout(textLayout);
        s_Renderer2DData.TextVertexArray->AddVertexBuffer(s_Renderer2DData.TextVertexBuffer);
        s_Renderer2DData.TextVertexArray->SetIndexBuffer(quadIB);

        s_Renderer2DData.WhiteTexture = Texture2D::Create(1, 1, ImageFormat::RGBA8);
        uint32_t whiteTextureData = 0xffffffff;
        s_Renderer2DData.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_Renderer2DData.QuadShader = CreateRef<Shader>("QuadShader", std::string(quadShaderSource));
        s_Renderer2DData.TextShader = CreateRef<Shader>("TextShader", std::string(textShaderSource));
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

                batch.TextureSlots[0] = s_Renderer2DData.WhiteTexture;

                for(uint32_t i = 0; i < batch.TextureSlotIndex; i++)
                {
                    batch.TextureSlots[i]->Bind(i);
                }

                s_Renderer2DData.QuadShader->Bind();
                RendererAPI::DrawIndexed(s_Renderer2DData.QuadVertexArray, batch.QuadIndexCount);
            }

            if(batch.TextIndexCount > 0)
            {
                s_Renderer2DData.TextVertexBuffer->SetData(batch.TextVertices.data(), batch.TextVertices.size() * sizeof(TextVertex));

                batch.FontAtlasTexture->Bind(0);

                s_Renderer2DData.TextShader->Bind();
                RendererAPI::DrawIndexed(s_Renderer2DData.TextVertexArray, batch.TextIndexCount);
            }
        }

        forwardBuffer->UnBind();

        s_Renderer2DData.Batches.clear();
    }

    void Renderer2D::Shutdown()
    {
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), {position.x, position.y, 0.0f})
                            * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        
        DrawQuad(transform, color);
    }

/*     void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), {position.x, position.y, 0.0f})
                            * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        
        DrawQuad(transform, texture, tilingFactor, tintColor);
    } */

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

        // Convert entityID to vec3
        uint32_t r = (entityID & 0x000000FF) >> 0;
        uint32_t g = (entityID & 0x0000FF00) >> 8;
        uint32_t b = (entityID & 0x00FF0000) >> 16;
        glm::vec3 entityIDVec3 = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);

        for(size_t i = 0; i < quadVertexCount; i++)
        {
            batch.QuadVertices.push_back(
            {
                transform * s_Renderer2DData.QuadVertexPositions[i], 
                color, 
                texCoords[i], 
                0.0f, 
                1.0f, 
                entityIDVec3
                });
        }

        batch.QuadIndexCount += 6;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, uint32_t entityID)
    {
        constexpr size_t quadVertexCount = 4;
        constexpr glm::vec2 texCoords[] = {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f}
        };

        if(s_Renderer2DData.Batches.empty())
        {
            s_Renderer2DData.Batches.push_back(Batch());
        }

        Batch& batch = s_Renderer2DData.Batches.back();

        if(batch.QuadIndexCount >= Batch::MaxIndices)
        {
            s_Renderer2DData.Batches.push_back(Batch());
            batch = s_Renderer2DData.Batches.back();
        }

        float textureIndex = 0.0f;
        for(uint32_t i = 1; i < batch.TextureSlotIndex; i++)
        {
            if(batch.TextureSlots[i] == texture)
            {
                textureIndex = (float)i;
                break;
            }
        }

        if(textureIndex == 0.0f)
        {
            if(batch.TextureSlotIndex >= Batch::MaxTextureSlots)
            {
                s_Renderer2DData.Batches.push_back(Batch());
                batch = s_Renderer2DData.Batches.back();
            }

            textureIndex = (float)batch.TextureSlotIndex;
            batch.TextureSlots[batch.TextureSlotIndex] = texture;
            batch.TextureSlotIndex++;
        }

        // Convert entityID to vec3
        uint32_t r = (entityID & 0x000000FF) >> 0;
        uint32_t g = (entityID & 0x0000FF00) >> 8;
        uint32_t b = (entityID & 0x00FF0000) >> 16;
        glm::vec3 entityIDVec3 = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);

        for(size_t i = 0; i < quadVertexCount; i++)
        {
            batch.QuadVertices.push_back(
            {
                transform * s_Renderer2DData.QuadVertexPositions[i], 
                tintColor, 
                texCoords[i], 
                textureIndex, 
                tilingFactor, 
                entityIDVec3
                });
        }

        batch.QuadIndexCount += 6;
    }

    void Renderer2D::DrawTextW(const std::string &text, Ref<Font> font, const glm::mat4 &transform, const TextParams &textParams, uint32_t entityID)
    {
        if(s_Renderer2DData.Batches.empty())
        {
            s_Renderer2DData.Batches.push_back(Batch());
        }

        Batch& batch = s_Renderer2DData.Batches.back();

        if(batch.TextIndexCount >= Batch::MaxIndices)
        {
            s_Renderer2DData.Batches.push_back(Batch());
            batch = s_Renderer2DData.Batches.back();
        }

        const auto& fontGeometry = font->GetMSDFData()->FontGeometry;
        const auto& metrics = fontGeometry.getMetrics();
        Ref<Texture2D> fontAtlas = font->GetAtlasTexture();
        
        // TODO: Skip to the next batch if the font is different
        batch.FontAtlasTexture = fontAtlas;

        double x = 0.0;
		double fsScale = 1.0 / (metrics.ascenderY - metrics.descenderY);
		double y = 0.0;

		const float spaceGlyphAdvance = fontGeometry.getGlyph(' ')->getAdvance();

        for (size_t i = 0; i < text.size(); i++)
		{
			char character = text[i];
			if (character == '\r')
				continue;

			if (character == '\n')
			{
				x = 0;
				y -= fsScale * metrics.lineHeight + textParams.LineSpacing;
				continue;
			}

			if (character == ' ')
			{
				float advance = spaceGlyphAdvance;
				if (i < text.size() - 1)
				{
					char nextCharacter = text[i + 1];
					double dAdvance;
					fontGeometry.getAdvance(dAdvance, character, nextCharacter);
					advance = (float)dAdvance;
				}

				x += fsScale * advance + textParams.Kerning;
				continue;
			}

			if (character == '\t')
			{
				// NOTE(Yan): is this right?
				x += 4.0f * (fsScale * spaceGlyphAdvance + textParams.Kerning);
				continue;
			}

			auto glyph = fontGeometry.getGlyph(character);
			if (!glyph)
				glyph = fontGeometry.getGlyph('?');
			if (!glyph)
				return;

			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin((float)al, (float)ab);
			glm::vec2 texCoordMax((float)ar, (float)at);

			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin((float)pl, (float)pb);
			glm::vec2 quadMax((float)pr, (float)pt);

			quadMin *= fsScale, quadMax *= fsScale;
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);

			float texelWidth = 1.0f / fontAtlas->GetWidth();
			float texelHeight = 1.0f / fontAtlas->GetHeight();
			texCoordMin *= glm::vec2(texelWidth, texelHeight);
			texCoordMax *= glm::vec2(texelWidth, texelHeight);

            // Convert entityID to vec3
            uint32_t r = (entityID & 0x000000FF) >> 0;
            uint32_t g = (entityID & 0x0000FF00) >> 8;
            uint32_t b = (entityID & 0x00FF0000) >> 16;
            glm::vec3 entityIDVec3 = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f);

            batch.TextVertices.push_back({
                transform * glm::vec4(quadMin, 0.0f, 1.0f),
                textParams.Color,
                texCoordMin,
                entityIDVec3
            });
            
            batch.TextVertices.push_back({
                transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f),
                textParams.Color,
                { texCoordMin.x, texCoordMax.y },
                entityIDVec3
            });
            
            batch.TextVertices.push_back({
                transform * glm::vec4(quadMax, 0.0f, 1.0f),
                textParams.Color,
                texCoordMax,
                entityIDVec3
            });
            
            batch.TextVertices.push_back({
                transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f),
                textParams.Color,
                { texCoordMax.x, texCoordMin.y },
                entityIDVec3
            });
            
            batch.TextIndexCount += 6;

            if (i < text.size() - 1)
			{
				double advance = glyph->getAdvance();
				char nextCharacter = text[i + 1];
				fontGeometry.getAdvance(advance, character, nextCharacter);

				x += fsScale * advance + textParams.Kerning;
			}
        }
    }
}