#pragma once

#include "CoffeeEngine/Renderer/Camera.h"
#include "CoffeeEngine/Renderer/EditorCamera.h"
#include "CoffeeEngine/Renderer/Framebuffer.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/VertexArray.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <vector>

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

        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        static void BeginScene(const EditorCamera& camera);
        static void EndScene();

        static void StartBatch();
        static void NextBatch();

        /**
         * @brief Flushes the batched vertices to the GPU and renders them.
         */
        static void Flush();

        /**
         * @brief Draws a line between two points.
         * @param start The starting point of the line.
         * @param end The ending point of the line.
         * @param color The color of the line. Default is white.
         * @param lineWidth The width of the line. Default is 1.0f.
         */
        static void DrawLine(const glm::vec3& start, const glm::vec3& end, glm::vec4 color = glm::vec4(1.0f), float lineWidth = 1.0f);

        /**
         * @brief Draws a circle.
         * @param position The center position of the circle.
         * @param radius The radius of the circle.
         * @param rotation The rotation of the circle.
         * @param color The color of the circle. Default is white.
         * @param lineWidth The width of the circle line. Default is 1.0f.
         */
        static void DrawCircle(const glm::vec3& position, float radius, const glm::quat& rotation, glm::vec4 color = glm::vec4(1.0f), float lineWidth = 1.0f);

        /**
         * @brief Draws a sphere.
         * @param position The center position of the sphere.
         * @param radius The radius of the sphere.
         * @param color The color of the sphere. Default is white.
         * @param lineWidth The width of the sphere line. Default is 1.0f.
         */
        static void DrawSphere(const glm::vec3& position, float radius, const glm::vec4& color = glm::vec4(1.0f), float lineWidth = 1.0f);

        /**
         * @brief Draws a box.
         * @param position The center position of the box.
         * @param rotation The rotation of the box.
         * @param size The size of the box.
         * @param color The color of the box. Default is white.
         * @param isCentered Whether the box is centered at the position.
         * @param lineWidth The width of the box line. Default is 1.0f.
         */
        static void DrawBox(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& size, const glm::vec4& color = glm::vec4(1.0f), const bool& isCentered = true, float lineWidth = 1.0f);

        /**
         * @brief Draws a box.
         * @param min The minimum corner of the box.
         * @param max The maximum corner of the box.
         * @param color The color of the box. Default is white.
         * @param lineWidth The width of the box line. Default is 1.0f.
         */
        static void DrawBox(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color = glm::vec4(1.0f), float lineWidth = 1.0f);

        /**
         * @brief Draws a box.
         * @param aabb The axis-aligned bounding box.
         * @param color The color of the box. Default is white.
         * @param lineWidth The width of the box line. Default is 1.0f.
         */
        static void DrawBox(const AABB& aabb, const glm::vec4& color = glm::vec4(1.0f), float lineWidth = 1.0f);

        /**
         * @brief Draws a box.
         * @param obb The oriented bounding box.
         * @param color The color of the box. Default is white.
         * @param lineWidth The width of the box line. Default is 1.0f.
         */
        static void DrawBox(const OBB& obb, const glm::vec4& color = glm::vec4(1.0f), float lineWidth = 1.0f);

        /**
         * @brief Draws an arrow.
         * @param start The starting point of the arrow.
         * @param end The ending point of the arrow.
         * @param fixedLength Whether the arrow has a fixed length.
         * @param color The color of the arrow. Default is white.
         * @param lineWidth The width of the arrow line. Default is 1.0f.
         */
        static void DrawArrow(const glm::vec3& start, const glm::vec3& end, bool fixedLength, glm::vec4 color = glm::vec4(1.0f), float lineWidth = 1.0f);

        /**
         * @brief Draws an arrow.
         * @param origin The origin point of the arrow.
         * @param direction The direction of the arrow.
         * @param length The length of the arrow.
         * @param color The color of the arrow. Default is white.
         * @param lineWidth The width of the arrow line. Default is 1.0f.
         */
        static void DrawArrow(const glm::vec3& origin, const glm::vec3& direction, float length, glm::vec4 color = glm::vec4(1.0f), float lineWidth = 1.0f);

    private:
        static Ref<VertexArray> m_LineVertexArray;
        static Ref<VertexBuffer> m_LineVertexBuffer;

        static Ref<VertexArray> m_CircleVertexArray;
        static Ref<VertexBuffer> m_CircleVertexBuffer;

        static Ref<Shader> m_DebugShader;

        static const int MaxVertices = 10000;
        static DebugVertex m_LineVertices[MaxVertices];
        static DebugVertex m_CircleVertices[MaxVertices];
        static int m_LineVertexCount;
        static int m_CircleVertexCount;

        //static Ref<Framebuffer> m_Framebuffer;
        //static Ref<Texture2D> m_RenderTexture;
    };

    /** @} */

}