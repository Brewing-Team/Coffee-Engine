#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/Math/BoundingBox.h"

#include <glm/glm.hpp>

#include <stdint.h>
#include <vector>

// Forward declarations
namespace Coffee {
    class VertexArray;
    class VertexBuffer;
    class IndexBuffer;
    class Material;
    class PBRMaterial;
    class ResourceLoader;
    struct ImportData;
    struct MeshImportData;
    struct AABB;
    struct OBB;
    struct UUID;
}

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
        glm::ivec4 BoneIDs = glm::ivec4(-1); ///< The bone IDs of the vertex.
        glm::vec4 BoneWeights = glm::vec4(0.0f); ///< The bone weights of the vertex.

    private:
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive& archive);
    };

    /**
     * @brief Class representing a mesh.
     */
    class Mesh : public Resource
    {
    public:
        /**
         * @brief Constructs a Mesh with the specified indices and vertices.
         * @param vertices The vertices of the mesh.
         * @param indices The indices of the mesh.
         */
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
        
        /**
         * @brief Constructs a Mesh from import data.
         * @param importData The import data containing mesh information.
         */
        Mesh(const ImportData& importData);

        /**
         * @brief Gets the vertex array of the mesh.
         * @return A reference to the vertex array.
         */
        const Ref<VertexArray>& GetVertexArray() const;

        /**
         * @brief Gets the vertex buffer of the mesh.
         * @return A reference to the vertex buffer.
         */
        const Ref<VertexBuffer>& GetVertexBuffer() const;

        /**
         * @brief Gets the index buffer of the mesh.
         * @return A reference to the index buffer.
         */
        const Ref<IndexBuffer>& GetIndexBuffer() const;

        /**
         * @brief Sets the material of the mesh.
         * @param material A reference to the material.
         */
        void SetMaterial(Ref<Material>& material);

        /**
         * @brief Sets the axis-aligned bounding box (AABB) of the mesh.
         * @param aabb The axis-aligned bounding box to set.
         */
        void SetAABB(const AABB& aabb);

        /**
         * @brief Gets the axis-aligned bounding box (AABB) of the mesh.
         * @return A reference to the AABB.
         */
        const AABB& GetAABB() const;

        /**
         * @brief Gets the oriented bounding box (OBB) of the mesh.
         * @param transform The transformation matrix.
         * @return The OBB.
         */
        OBB GetOBB(const glm::mat4& transform) const;

        /**
         * @brief Gets the material of the mesh.
         * @return A reference to the material.
         */
        const Ref<Material>& GetMaterial() const;

        /**
         * @brief Gets the vertices of the mesh.
         * @return A reference to the vector of vertices.
         */
        const std::vector<Vertex>& GetVertices() const;

        /**
         * @brief Gets the indices of the mesh.
         * @return A reference to the vector of indices.
         */
        const std::vector<uint32_t>& GetIndices() const;

    private:
        friend class cereal::access;

        template<class Archive>
        void save(Archive& archive) const;

        template<class Archive>
        void load(Archive& archive);

        template<class Archive>
        static void load_and_construct(Archive& data, cereal::construct<Mesh>& construct);

    private:
        Ref<VertexArray> m_VertexArray; ///< The vertex array of the mesh.
        Ref<VertexBuffer> m_VertexBuffer; ///< The vertex buffer of the mesh.
        Ref<IndexBuffer> m_IndexBuffer; ///< The index buffer of the mesh.

        Ref<Material> m_Material; ///< The material of the mesh.
        AABB m_AABB; ///< The axis-aligned bounding box of the mesh.

        std::vector<uint32_t> m_Indices; ///< The indices of the mesh.
        std::vector<Vertex> m_Vertices; ///< The vertices of the mesh.
    };

    /** @} */
}