#pragma once

#include "CoffeeEngine/Renderer/Mesh.h"

#include <memory>
#include <vector>

struct NavMeshTriangle
{
    glm::vec3 vertices[3];
    glm::vec3 center;
    glm::vec3 normal;
    std::vector<int> neighbors;
};

namespace Coffee
{
    class NavMesh
    {
    public:

        NavMesh() : m_WalkableSlopeAngle(45.0f) {}
        ~NavMesh() { Clear(); }

        void AddMesh(const std::shared_ptr<Mesh>& mesh, const glm::mat4& worldTransform);
        bool CalculateWalkableAreas();
        void RenderWalkableAreas() const;
        void Clear();

        const std::vector<NavMeshTriangle>& GetTriangles() const { return m_Triangles; }

    private:
        void ProcessMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const glm::mat4& transform);
        void CalculateNeighbors();
        bool AreTrianglesAdjacent(int triA, int triB) const;
        bool IsWalkableSurface(const glm::vec3& normal) const;

    private:
        struct MeshInfo
        {
            std::shared_ptr<Mesh> mesh;
            glm::mat4 worldTransform;
        };

        std::vector<MeshInfo> m_Meshes;
        std::vector<NavMeshTriangle> m_Triangles;
        float m_WalkableSlopeAngle;
    };
}