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
            OctreeNode(OctreeNode* parent, AABB aabb, uint32_t depth);
            ~OctreeNode();

            void Insert(ObjectContainer object);
            void Subdivide(int depth);
            void DebugDrawAABB();

        public:
            OctreeNode* parent;
            OctreeNode* children[8];
            std::list<ObjectContainer> objectList; ///< list of objects of the node
            AABB aabb;
            uint32_t depth;
            uint32_t maxDepth = 4;
            uint32_t maxObjectsPerNode = 8;
    };

    class Octree
    {
        public:
            Octree(AABB bounds);
            ~Octree();

            void Insert(const glm::vec3& position, glm::vec3 data /*TODO THIS SHOULD BE A TEMPLATE*/);
            void Update();

        public:
            uint32_t maxDepth = 4;
            OctreeNode* rootNode;
    };

}