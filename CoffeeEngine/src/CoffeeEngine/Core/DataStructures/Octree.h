#pragma once
#include "CoffeeEngine/Renderer/Mesh.h"

#include <list>

namespace Coffee {

    struct MeshComponent; ///< Forward declaration of MeshComponent.

    struct ObjectContainer
    {
        int id;
        glm::vec3 position;
        // TODO add templates for different types of objects
    };

    struct OctreeNode
    {
        public:
            OctreeNode() = default;
            OctreeNode(const AABB& aabb) : aabb(aabb) {}
            ~OctreeNode()
            {
                for (auto& child : children)
                {
                    if (child)
                    {
                        child.reset();
                    }
                }
            }

            void DebugDrawAABB();
            int GetChildIndex(const AABB& bounds, const glm::vec3& point) const;

        public:
            Scope<OctreeNode> children[8]; ///< Pointers to child nodes
            std::vector<ObjectContainer> objectList; ///< list of objects of the node
            AABB aabb;
            bool isLeaf = true;
    };

    class Octree
    {
        public:
            Octree(const AABB& bounds, int maxObjectsPerNode = 8, int maxDepth = 5);
            ~Octree();

            void Insert(const ObjectContainer& object);
            void Subdivide(OctreeNode& node);
            void Clear();
            void DebugDraw();

        private:
            void Insert(OctreeNode& node, const ObjectContainer& object);

        public:
            uint32_t maxDepth = 4;
            uint32_t maxObjectsPerNode = 8;
            OctreeNode rootNode;
    };

}