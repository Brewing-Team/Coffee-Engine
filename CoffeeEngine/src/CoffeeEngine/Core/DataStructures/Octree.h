#pragma once
#include "CoffeeEngine/Renderer/Mesh.h"

#include <list>

namespace Coffee {

    struct MeshComponent; ///< Forward declaration of MeshComponent.

    struct ObjectContainer
    {
        glm::vec3 position;
        // TODO add templates
        glm::vec3 data;
    };

    struct OctreeNode
    {
        public:
            OctreeNode(OctreeNode* parent, AABB aabb);
            ~OctreeNode();

            void Insert(ObjectContainer object);
            void Preallocate(int depth);
            void DebugDrawAABB();
            bool IsLeaf() const;

        public:
            OctreeNode* parent;
            OctreeNode* children[8];
            std::list<ObjectContainer> objectList; ///< list of objects of the node
            AABB aabb;
            int depth;
    };

    class Octree
    {
        public:
            Octree(AABB bounds);
            ~Octree();

            void Preallocate(int depth);
            void Insert(const glm::vec3& position, glm::vec3 data /*TODO THIS SHOULD BE A TEMPLATE*/);
            void Update();

        public:
            uint32_t maxDepth = 4;
            uint32_t maxObjectsPerNode = 8;
            OctreeNode* rootNode;
    };

}