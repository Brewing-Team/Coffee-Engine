#pragma once

#include "NavMesh.h"
#include "CoffeeEngine/Renderer/DebugRenderer.h"

#include <vector>

namespace Coffee
{
    struct NavMeshPortal
    {
        glm::vec3 left;
        glm::vec3 right;
    };

    class NavMeshPathfinding
    {
    public:
        NavMeshPathfinding(const Ref<NavMesh>& navMesh)
            : m_NavMesh(navMesh) {}

        std::vector<glm::vec3> FindPath(const glm::vec3& start, const glm::vec3& end) const;
        void RenderPath(const std::vector<glm::vec3>& path) const;

    private:
        int FindTriangleContaining(const glm::vec3& point) const;
        float Heuristic(int current, int goal, const std::vector<NavMeshTriangle>& triangles) const;
        bool IsPointInTriangle(const glm::vec3& point, const NavMeshTriangle& triangle) const;
        glm::vec3 ProjectPointOnPlane(const glm::vec3& point, const NavMeshTriangle& triangle) const;
        NavMeshPortal GetPortal(const NavMeshTriangle& fromTri, const NavMeshTriangle& toTri) const;
        std::vector<glm::vec3> StringPull(const std::vector<NavMeshPortal>& portals, const glm::vec3& start, const glm::vec3& end) const;
        glm::vec3 FindClosestPointOnTriangle(const glm::vec3& point, const NavMeshTriangle& triangle) const;
        float DistanceToTriangle(const glm::vec3& point, const NavMeshTriangle& triangle) const;
        float TriArea2D(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) const;

    private:
        Ref<NavMesh> m_NavMesh;
    };
}