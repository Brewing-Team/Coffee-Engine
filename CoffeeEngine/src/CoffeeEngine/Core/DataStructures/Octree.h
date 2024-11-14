#pragma once
#include "CoffeeEngine/Scene/Entity.h"
#include <glm/detail/func_common.inl>

class Octree {
};

struct OctreeNode {
    glm::vec3 center; ///< Center point of the node
    float halfWidth; ///< Half the width of the node volume
    OctreeNode* children[8]; ///< Pointers to the 8 children nodes
    Octree* objectList; ///< Linked list of objects contained at this node
    Coffee::Entity* pNextObject; ///< Pointer to the next object when linked into list

    OctreeNode* BuildOctree(glm::vec3 center, float halfWidth, int stopDepth);
    void InsertObject(OctreeNode* pTree, Coffee::Entity* pObject);
    void TestAllCollisions(OctreeNode* pTree);
};