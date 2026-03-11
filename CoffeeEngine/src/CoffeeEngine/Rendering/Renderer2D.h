#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Rendering/RendererAPI.h"

#include <glm/gtc/quaternion.hpp>

#include <queue>
#include <string>

namespace Coffee {

    struct Batch;
    struct AABB;
    struct OBB;

    class Frustum;
    class Font;
    class Texture2D;
    class RenderTarget;

    class VertexArray;
    class VertexBuffer;

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
        static const uint32_t MaxQuadCount = 20000; // Think of increasing this number to 20000
        static const uint32_t MaxVertices = MaxQuadCount * 4;
        static const uint32_t MaxIndices = MaxQuadCount * 6;
        static const uint32_t MaxTextureSlots = 64;

        std::vector<QuadVertex> QuadVertices;
        uint32_t QuadIndexCount = 0;

        std::vector<LineVertex> LineVertices;

        std::vector<TextVertex> TextVertices;
        uint32_t TextIndexCount = 0;

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint32_t TextureSlotIndex = 1; // 0 is reserved for white texture

        Ref<Texture2D> FontAtlasTexture;

        float LineWidth = 1.5f;
    };

    struct Renderer2DData
    {
        std::queue<Batch> WorldBatches;
        std::queue<Batch> ScreenBatches;

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
    
    class Renderer2D
    {
    public:
        enum class RenderMode
        {
            World,
            Screen
        };
    public:
        void Init(RendererAPI* api);

        /*
        Render();

        or

        RenderWorld();
        RenderUI();

        or

        WorldPass();
        UIPass();
        */

        void WorldPass(const Ref<RenderTarget>& target);
        void ScreenPass(const Ref<RenderTarget>& target);

        void Shutdown();

        void DrawRect(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), uint32_t entityID = 4294967295);
        void DrawRect(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color = glm::vec4(1.0f), uint32_t entityID = 4294967295);
        // Billboard?
        void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f), uint32_t entityID = 4294967295);
        void DrawRect(const glm::mat4& transform, const glm::vec4& color, RenderMode mode, uint32_t entityID = 4294967295);

        void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        void DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor, RenderMode mode, uint32_t entityID = 4294967295, const glm::vec4& uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

        void DrawLine(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color = glm::vec4(1.0f), float linewidth = 1.0f);
        void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color = glm::vec4(1.0f), float linewidth = 1.0f);

        void DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color = glm::vec4(1.0f), float linewidth = 1.0f);
        void DrawCircle(const glm::vec3& position, float radius, const glm::quat& rotation = glm::identity<glm::quat>(), const glm::vec4& color = glm::vec4(1.0f), float linewidth = 1.0f);

        void DrawSphere(const glm::vec3& position, float radius, const glm::quat& rotation = glm::identity<glm::quat>(), const glm::vec4& color = glm::vec4(1.0f), float linewidth = 1.0f);

        void DrawBox(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& size, const glm::vec4& color = glm::vec4(1.0f), const bool& isCentered = true, float linewidth = 1.0f);
        void DrawBox(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color = glm::vec4(1.0f), float lineWidth = 1.0f);
        void DrawBox(const AABB& aabb, const glm::vec4& color = glm::vec4(1.0f), float lineWidth = 1.0f);
        void DrawBox(const OBB& obb, const glm::vec4& color = glm::vec4(1.0f), float lineWidth = 1.0f);
        
        void DrawArrow(const glm::vec3& start, const glm::vec3& end, bool fixedLength, glm::vec4 color = glm::vec4(1.0f), float lineWidth = 1.0f);
        void DrawArrow(const glm::vec3& origin, const glm::vec3& direction, float length, glm::vec4 color = glm::vec4(1.0f), float lineWidth = 1.0f);

        void DrawFrustum(const Frustum& frustum, const glm::vec4& color = glm::vec4(1.0f), float lineWidth = 1.0f);
        void DrawFrustum(const glm::mat4& viewProjection, const glm::vec4& color = glm::vec4(1.0f), float lineWidth = 1.0f);

        void DrawCapsule(const glm::vec3& position, const glm::quat& rotation, float radius, float height, const glm::vec4& color = glm::vec4(1.0f));
        
        void DrawCylinder(const glm::vec3& position, const glm::quat& rotation, float radius, float height,
                                 const glm::vec4& color = glm::vec4(1.0f));

        void DrawCone(glm::vec3 vec, glm::quat qua, float radius, float height, glm::vec4 vec4);
        void DrawTruncatedCone(glm::vec3 position, glm::quat rotation, float baseRadius, float topRadius,
                                      float height, glm::vec4 color);

        enum class TextAlignment
        {
            Left,
            Center,
            Right
        };

        struct TextParams
		{
			glm::vec4 Color{ 1.0f };
			float Kerning = 0.0f;
			float LineSpacing = 0.0f;
            float Size = 16.0f;
            TextAlignment Alignment;
		};

        void DrawTextString(const std::string& text, Ref<Font> font, const glm::mat4& transform, const TextParams& textParams, RenderMode mode, uint32_t entityID = 4294967295);
    private:
        Batch& GetBatch(RenderMode mode);
        void NextBatch(RenderMode mode);
    private:
        RendererAPI* m_API;
        Renderer2DData m_Renderer2DData;
    };

}