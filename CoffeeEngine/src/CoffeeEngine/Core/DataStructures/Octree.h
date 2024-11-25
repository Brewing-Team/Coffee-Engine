#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include <vector>
#include <memory>

namespace Coffee {

    struct ObjectContainer
    {
        glm::vec3 position;
        // TODO add templates for different types of objects
    };

    class OctreeNode
    {
    public:
        AABB aabb;
        bool isLeaf = true;
        std::vector<ObjectContainer> objectList;
        std::array<Scope<OctreeNode>, 8> children;

        void DebugDrawAABB();
        int GetChildIndex(const AABB& bounds, const glm::vec3& point) const;
    };

    class Octree
    {
    public:
        Octree(const AABB& bounds, int maxObjectsPerNode = 8, int maxDepth = 5);
        ~Octree();

        void Insert(const ObjectContainer& object);
        void DebugDraw();
        void Clear();

    private:
        void Insert(OctreeNode& node, const ObjectContainer& object);
        void InsertIntoLeaf(OctreeNode& node, const ObjectContainer& object);
        void InsertIntoChild(OctreeNode& node, const ObjectContainer& object);
        void RedistributeObjects(OctreeNode& node);
        void Subdivide(OctreeNode& node);
        void CreateChildren(OctreeNode& node, const glm::vec3& center);

        OctreeNode rootNode;
        int maxObjectsPerNode;
        int maxDepth;
    };

} // namespace Coffee