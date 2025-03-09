#include "NavMesh.h"

#include "CoffeeEngine/Renderer/DebugRenderer.h"

#include <algorithm>
#include <cmath>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

namespace Coffee
{
    void NavMesh::AddMesh(const std::shared_ptr<Mesh>& mesh, const glm::mat4& worldTransform)
    {
        if (!mesh)
            return;

        MeshInfo info;
        info.mesh = mesh;
        info.worldTransform = worldTransform;
        m_Meshes.push_back(info);
    }

    bool NavMesh::CalculateWalkableAreas()
    {
        m_Triangles.clear();

        if (m_Meshes.empty())
            return false;

        for (const auto& [mesh, worldTransform] : m_Meshes)
        {
            if (!mesh)
                continue;

            ProcessMesh(mesh->GetVertices(), mesh->GetIndices(), worldTransform);
        }

        CalculateNeighbors();

        return !m_Triangles.empty();
    }

    void NavMesh::ProcessMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const glm::mat4& transform)
    {
        if (vertices.empty() || indices.empty())
            return;

        if (indices.size() % 3 != 0)
            return;

        int triangleCount = indices.size() / 3;
        for (int i = 0; i < triangleCount; i++)
        {
            uint32_t idx0 = indices[i * 3];
            uint32_t idx1 = indices[i * 3 + 1];
            uint32_t idx2 = indices[i * 3 + 2];

            if (idx0 >= vertices.size() || idx1 >= vertices.size() || idx2 >= vertices.size())
                continue;

            glm::vec3 v0 = glm::vec3(transform * glm::vec4(vertices[idx0].Position, 1.0f));
            glm::vec3 v1 = glm::vec3(transform * glm::vec4(vertices[idx1].Position, 1.0f));
            glm::vec3 v2 = glm::vec3(transform * glm::vec4(vertices[idx2].Position, 1.0f));

            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            if (!IsWalkableSurface(normal))
                continue;

            NavMeshTriangle tri;
            tri.vertices[0] = v0;
            tri.vertices[1] = v1;
            tri.vertices[2] = v2;
            tri.normal = normal;
            tri.center = (v0 + v1 + v2) / 3.0f;

            m_Triangles.push_back(tri);
        }
    }

    void NavMesh::RenderWalkableAreas() const
    {
        constexpr glm::vec4 edgeColor(0.2f, 0.7f, 1.0f, 1.0f);

        for (const auto& tri : m_Triangles)
        {
            for (int i = 0; i < 3; i++)
            {
                const int next = (i + 1) % 3;
                DebugRenderer::DrawLine(tri.vertices[i], tri.vertices[next], edgeColor, 20.0f);
            }
        }
    }

    void NavMesh::Clear()
    {
        m_Meshes.clear();
        m_Triangles.clear();
    }

    void NavMesh::CalculateNeighbors()
    {
        for (int i = 0; i < m_Triangles.size(); i++)
        {
            for (int j = i + 1; j < m_Triangles.size(); j++)
            {
                if (AreTrianglesAdjacent(i, j))
                {
                    m_Triangles[i].neighbors.push_back(j);
                    m_Triangles[j].neighbors.push_back(i);
                }
            }
        }
    }

    bool NavMesh::AreTrianglesAdjacent(int triA, int triB) const
    {
        if (triA == triB || triA >= m_Triangles.size() || triB >= m_Triangles.size())
            return false;

        int sharedVertices = 0;

        constexpr float epsilon = 0.001f;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (glm::distance2(m_Triangles[triA].vertices[i], m_Triangles[triB].vertices[j]) < epsilon * epsilon)
                {
                    sharedVertices++;
                    break;
                }
            }
        }

        return sharedVertices == 2;
    }

    bool NavMesh::IsWalkableSurface(const glm::vec3& normal) const
    {
        if (std::abs(glm::length(normal) - 1.0f) > 0.001f)
            return false;

        constexpr glm::vec3 upVector(0.0f, 1.0f, 0.0f);

        float dotProduct = glm::dot(normal, upVector);
        float angleRadians = std::acos(dotProduct);
        float angleDegrees = glm::degrees(angleRadians);

        return angleDegrees <= m_WalkableSlopeAngle;
    }
}