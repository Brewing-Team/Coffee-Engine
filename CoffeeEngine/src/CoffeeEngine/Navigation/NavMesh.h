#pragma once

#include "CoffeeEngine/Renderer/Mesh.h"

#include <memory>
#include <vector>

/**
 * @brief Structure representing a triangle in the navigation mesh.
 */
struct NavMeshTriangle
{
    glm::vec3 vertices[3]; ///< Vertices of the triangle
    glm::vec3 center; ///< Center of the triangle
    glm::vec3 normal; ///< Normal of the triangle
    std::vector<int> neighbors; ///< Indices of neighboring triangles
};

namespace Coffee
{
    /**
     * @brief Class representing a navigation mesh.
     */
    class NavMesh
    {
    public:
        /**
         * @brief Constructor.
         */
        NavMesh() : m_WalkableSlopeAngle(45.0f) {}

        /**
         * @brief Destructor.
         */
        ~NavMesh() { Clear(); }

        /**
         * @brief Calculates walkable areas from the given mesh and world transform.
         * @param mesh The mesh to process.
         * @param worldTransform The world transform of the mesh.
         * @return True if the walkable areas were successfully calculated, false otherwise.
         */
        bool CalculateWalkableAreas(const std::shared_ptr<Mesh>& mesh, const glm::mat4& worldTransform);

        /**
         * @brief Renders the walkable areas for debugging purposes.
         */
        void RenderWalkableAreas() const;

        /**
         * @brief Clears the navigation mesh data.
         */
        void Clear();

        /**
         * @brief Returns the triangles in the navigation mesh.
         * @return A reference to the vector of triangles.
         */
        const std::vector<NavMeshTriangle>& GetTriangles() const { return m_Triangles; }

        /**
         * @brief Checks if the navigation mesh has been calculated.
         * @return True if the navigation mesh has been calculated, false otherwise.
         */
        bool IsCalculated() const { return m_Calculated; }

    private:
        /**
         * @brief Processes the mesh data to generate navigation mesh triangles.
         * @param vertices The vertices of the mesh.
         * @param indices The indices of the mesh.
         * @param transform The transformation matrix.
         */
        void ProcessMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const glm::mat4& transform);

        /**
         * @brief Calculates the neighbors for each triangle in the navigation mesh.
         */
        void CalculateNeighbors();

        /**
         * @brief Checks if two triangles are adjacent.
         * @param triA The index of the first triangle.
         * @param triB The index of the second triangle.
         * @return True if the triangles are adjacent, false otherwise.
         */
        bool AreTrianglesAdjacent(int triA, int triB) const;

        /**
         * @brief Checks if a surface is walkable based on its normal.
         * @param normal The normal of the surface.
         * @return True if the surface is walkable, false otherwise.
         */
        bool IsWalkableSurface(const glm::vec3& normal) const;

    private:
        /**
         * @brief Structure to hold mesh information.
         */
        struct MeshInfo
        {
            std::shared_ptr<Mesh> mesh; ///< The mesh
            glm::mat4 worldTransform; ///< The world transform of the mesh
        };

        std::vector<NavMeshTriangle> m_Triangles; ///< Triangles in the navigation mesh
        float m_WalkableSlopeAngle; ///< Maximum walkable slope angle
        bool m_Calculated = false; ///< Flag indicating if the navigation mesh has been calculated
    };
}