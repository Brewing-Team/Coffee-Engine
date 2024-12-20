#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Buffer.h"
#include "CoffeeEngine/Renderer/Material.h"
#include "CoffeeEngine/Renderer/VertexArray.h"

#include <array>
#include <assimp/mesh.h>
#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Coffee {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the CoffeeEngine.
     * @{
     */

    /**
     * @brief Structure representing a vertex in a mesh.
     */
    struct Vertex {
        glm::vec3 Position = glm::vec3(0.0f); ///< The position of the vertex.
        glm::vec2 TexCoords = glm::vec2(0.0f); ///< The texture coordinates of the vertex.
        glm::vec3 Normals = glm::vec3(0.0f); ///< The normal vector of the vertex.
        glm::vec3 Tangent = glm::vec3(0.0f); ///< The tangent vector of the vertex.
        glm::vec3 Bitangent = glm::vec3(0.0f); ///< The bitangent vector of the vertex.
    };

    /**
     * @brief Structure representing an axis-aligned bounding box (AABB).
     */
    struct AABB {

        glm::vec3 min = glm::vec3(0.0f); ///< The minimum point of the AABB.
        glm::vec3 max = glm::vec3(0.0f); ///< The maximum point of the AABB.

        AABB() = default;

        /**
         * @brief Constructs an AABB with specified minimum and maximum points.
         * @param min The minimum point of the AABB.
         * @param max The maximum point of the AABB.
         */
        AABB(const glm::vec3& min, const glm::vec3& max)
            : min(min), max(max) {}
    };

    /**
     * @brief Structure representing an oriented bounding box (OBB).
     */
    struct OBB
    {
        std::array<glm::vec3, 8> corners; ///< The corners of the OBB.

        OBB() = default;

        /**
         * @brief Constructs an OBB with specified corners.
         * @param corners The corners of the OBB.
         */
        OBB(const std::array<glm::vec3, 8>& corners)
            : corners(corners) {}

        /**
         * @brief Constructs an OBB from a transformation matrix and an AABB.
         * @param transform The transformation matrix.
         * @param aabb The axis-aligned bounding box.
         */
        OBB(const glm::mat4& transform, const AABB& aabb)
            : corners({
                glm::vec3(transform * glm::vec4(aabb.min.x, aabb.min.y, aabb.min.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.max.x, aabb.min.y, aabb.min.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.max.x, aabb.max.y, aabb.min.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.min.x, aabb.max.y, aabb.min.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.min.x, aabb.min.y, aabb.max.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.max.x, aabb.min.y, aabb.max.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.max.x, aabb.max.y, aabb.max.z, 1.0f)),
                glm::vec3(transform * glm::vec4(aabb.min.x, aabb.max.y, aabb.max.z, 1.0f))
                }) {}
    };

    /**
     * @brief Class representing a mesh.
     */
    class Mesh
    {
    public:
        /**
         * @brief Constructs a Mesh with the specified indices and vertices.
         * @param indices The indices of the mesh.
         * @param vertices The vertices of the mesh.
         */
        Mesh(const std::vector<uint32_t>& indices, const std::vector<Vertex>& vertices);

        /**
         * @brief Gets the vertex array of the mesh.
         * @return A reference to the vertex array.
         */
        const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

        /**
         * @brief Gets the vertex buffer of the mesh.
         * @return A reference to the vertex buffer.
         */
        const Ref<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }

        /**
         * @brief Gets the index buffer of the mesh.
         * @return A reference to the index buffer.
         */
        const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

        /**
         * @brief Sets the name of the mesh.
         * @param name The name of the mesh.
         */
        void SetName(const std::string& name) { m_Name = name; }

        /**
         * @brief Gets the name of the mesh.
         * @return The name of the mesh.
         */
        const std::string& GetName() const { return m_Name; }

        /**
         * @brief Sets the material of the mesh.
         * @param material A reference to the material.
         */
        void SetMaterial(Ref<Material>& material) { m_Material = material; }

        /**
         * @brief Sets the axis-aligned bounding box (AABB) of the mesh.
         * @param aabb The axis-aligned bounding box to set.
         */
        void SetAABB(const AABB aabb) { m_AABB = aabb; }

        /**
         * @brief Gets the axis-aligned bounding box (AABB) of the mesh.
         * @return A reference to the AABB.
         */
        const AABB& GetAABB() { return m_AABB; }

        /**
         * @brief Gets the transformed axis-aligned bounding box (AABB) of the mesh.
         * @param transform The transformation matrix.
         * @return The transformed AABB.
         */
        AABB GetAABB(const glm::mat4& transform)
        {
            const AABB& aabb = GetAABB();

            // Compute the 8 corners of the AABB
            glm::vec3 corners[8] = {
                aabb.min,
                glm::vec3(aabb.min.x, aabb.min.y, aabb.max.z),
                glm::vec3(aabb.min.x, aabb.max.y, aabb.min.z),
                glm::vec3(aabb.min.x, aabb.max.y, aabb.max.z),
                glm::vec3(aabb.max.x, aabb.min.y, aabb.min.z),
                glm::vec3(aabb.max.x, aabb.min.y, aabb.max.z),
                glm::vec3(aabb.max.x, aabb.max.y, aabb.min.z),
                aabb.max
            };

            // Transform the corners
            glm::vec3 transformedCorners[8];
            for (int i = 0; i < 8; ++i) {
                transformedCorners[i] = glm::vec3(transform * glm::vec4(corners[i], 1.0f));
            }

            // Find the new min and max points
            glm::vec3 newMin = transformedCorners[0];
            glm::vec3 newMax = transformedCorners[0];

            for (int i = 1; i < 8; ++i) {
                newMin = glm::min(newMin, transformedCorners[i]);
                newMax = glm::max(newMax, transformedCorners[i]);
            }

            // Create the transformed AABB
            AABB transformedAABB(newMin, newMax);

            return transformedAABB;
        }

        /**
         * @brief Gets the oriented bounding box (OBB) of the mesh.
         * @param transform The transformation matrix.
         * @return The OBB.
         */
        OBB GetOBB(const glm::mat4& transform) { return {transform, GetAABB()}; }

        /**
         * @brief Gets the material of the mesh.
         * @return A reference to the material.
         */
        const Ref<Material>& GetMaterial() const { return m_Material; }

        /**
         * @brief Gets the vertices of the mesh.
         * @return A reference to the vector of vertices.
         */
        const std::vector<Vertex>& GetVertices() const { return m_Vertices; }

        /**
         * @brief Gets the indices of the mesh.
         * @return A reference to the vector of indices.
         */
        const std::vector<uint32_t>& GetIndices() const { return m_Indices; }

        /**
         * @brief Serializes the mesh to an archive.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template<class Archive>
        void save(Archive& archive)
        {
            archive(m_Name, m_Vertices, m_Indices);
        }

        /**
         * @brief Deserializes the mesh from an archive.
         * @tparam Archive The type of the archive.
         * @param archive The archive to deserialize from.
         */
        template <class Archive> void load(Archive& archive)
        {
            archive(m_Name, m_Vertices, m_Indices);
            Mesh(m_Indices, m_Vertices);
        }

      private:
        Ref<VertexArray> m_VertexArray; ///< The vertex array of the mesh.
        Ref<VertexBuffer> m_VertexBuffer; ///< The vertex buffer of the mesh.
        Ref<IndexBuffer> m_IndexBuffer; ///< The index buffer of the mesh.

        std::string m_Name; ///< The name of the mesh.

        Ref<Material> m_Material; ///< The material of the mesh.
        AABB m_AABB; ///< The axis-aligned bounding box of the mesh.

        std::vector<uint32_t> m_Indices; ///< The indices of the mesh.
        std::vector<Vertex> m_Vertices; ///< The vertices of the mesh.
    };

    /** @} */
}