#pragma once

#include "CoffeeEngine/Renderer/RenderTarget.h"
#include <glm/fwd.hpp>


namespace Coffee {
    
    class Renderer2D
    {
    public:
        static void Init();

        //TEMPORAL
        static void Render(const RenderTarget& target);

        /*
        static Render();

        or

        static RenderWorld();
        static RenderUI();

        or

        static WorldPass();
        static UIPass();
        */
        static void Shutdown();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
        
        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, uint32_t entityID = 4294967295);
        static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f), uint32_t entityID = 4294967295);
    };

}