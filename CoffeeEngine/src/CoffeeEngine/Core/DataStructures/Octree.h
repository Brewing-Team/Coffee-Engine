#pragma once
#include <glm/vec3.hpp>

class Octree {

};

struct OctreeNode {
    glm::vec3 center; ///< Center point of the node
    float halfWidth; ///< Half the width of the node volume
    OctreeNode* children[8]; ///< Pointers to the 8 children nodes
    Octree* objectList; ///< Linked list of objects contained at this node
};
