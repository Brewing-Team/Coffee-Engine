#pragma once

#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/VertexArray.h"
#include <glm/glm.hpp>

namespace Coffee {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the CoffeeEngine.
     * @{
     */

    /**
     * @brief Structure representing a vertex in a line.
     */
    struct DebugVertex
    {
        glm::vec3 Position; ///< The position of the vertex.
        glm::vec4 Color; ///< The color of the vertex.
    };

    /**
     * @brief Class responsible for rendering debug lines.
     */
    class DebugRenderer
    {
    public:
        /**
         * @brief Initializes the DebugRenderer.
         */
        static void Init();

        /**
         * @brief Shuts down the DebugRenderer.
         */
        static void Shutdown();

        /**
         * @brief Draws a line between two points.
         * @param start The starting point of the line.
         * @param end The ending point of the line.
         * @param color The color of the line. Default is white.
         * @param lineWidth The width of the line. Default is 1.0f.
         */
        static void DrawLine(const glm::vec3& start, const glm::vec3& end, glm::vec4 color = glm::vec4(1.0f), float lineWidth = 1.0f);

        static void DrawCircle(const glm::vec3& position, float radius, const glm::quat& rotation, glm::vec4 color = glm::vec4(1.0f), float lineWidth = 1.0f);

        static void DrawSphere(const glm::vec3& position, float radius, glm::vec4 color = glm::vec4(1.0f), float lineWidth = 1.0f);

        static void DrawArrow(const glm::vec3& start, const glm::vec3& end, bool fixedLength = true, glm::vec4 color = glm::vec4(1.0f), float lineWidth = 1.0f);
        static void DrawArrow(const glm::vec3& origin, const glm::vec3& direction, float length = 1.5f, glm::vec4 color = glm::vec4(1.0f), float lineWidth = 1.0f);

    private:
        static Ref<VertexArray> m_LineVertexArray; ///< The vertex array for the line.
        static Ref<VertexBuffer> m_LineVertexBuffer; ///< The vertex buffer for the line.

        static Ref<VertexArray> m_CircleVertexArray; ///< The vertex array for the circle.
        static Ref<VertexBuffer> m_CircleVertexBuffer; ///< The vertex buffer for the circle.

        static Ref<Shader> m_DebugShader; ///< The shader used for rendering the debug shapes.
    };

    /** @} */
}