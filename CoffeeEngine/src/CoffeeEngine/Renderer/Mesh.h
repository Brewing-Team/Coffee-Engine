#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/Renderer/Buffer.h"
#include "CoffeeEngine/Renderer/Material.h"
#include "CoffeeEngine/Renderer/VertexArray.h"
#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"

#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <array>
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>

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

        private:
            friend class cereal::access;

            template<class Archive>
            void serialize(Archive& archive)
            {
                archive(Position, TexCoords, Normals, Tangent, Bitangent);
            }    
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

        private:
            friend class cereal::access;

            template<class Archive>
            void serialize(Archive& archive)
            {
                archive(min, max);
            }
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
    class Mesh : public Resource
    {
    public:
        /**
         * @brief Constructs a Mesh with the specified indices and vertices.
         * @param indices The indices of the mesh.
         * @param vertices The vertices of the mesh.
         */
        Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

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

        // TODO doc
        static bool Intersects(const AABB& a, const glm::vec3& aPos, const AABB& b, const glm::vec3& bPos)
        {
            glm::vec3 aMin = a.min + aPos;
            glm::vec3 aMax = a.max + aPos;
            glm::vec3 bMin = b.min + bPos;
            glm::vec3 bMax = b.max + bPos;

            return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
                   (aMin.y <= bMax.y && aMax.y >= bMin.y) &&
                   (aMin.z <= bMax.z && aMax.z >= bMin.z);
        }

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

    private:
        friend class cereal::access;

        template<class Archive>
        void save(Archive& archive) const
        {
            archive(m_Vertices, m_Indices, m_Name, m_AABB, cereal::base_class<Resource>(this));
        }

        template<class Archive>
        void load(Archive& archive)
        {
            archive(m_Vertices, m_Indices, m_Name, m_AABB, cereal::base_class<Resource>(this));
        }

        template<class Archive>
        static void load_and_construct(Archive& data, cereal::construct<Mesh>& construct)
        {
            // Try to take this data as a reference
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            data(vertices, indices);
            construct(vertices, indices);

            data(construct->m_Name, construct->m_AABB, cereal::base_class<Resource>(construct.ptr()));
            construct->m_Vertices = vertices;
            construct->m_Indices = indices;
        }
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

CEREAL_REGISTER_TYPE(Coffee::Mesh);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Resource, Coffee::Mesh);