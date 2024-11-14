#include "Octree.h"

OctreeNode* OctreeNode::BuildOctree(glm::vec3 center, float halfWidth, int stopDepth)
{
    if (stopDepth < 0)
    {
        return nullptr;
    }
    else
    {
        // Construct and fill in 'root' of this subtree
        OctreeNode* pNode = new OctreeNode();
        pNode->center = center;
        pNode->halfWidth = halfWidth;
        pNode->objectList = nullptr;

        // Recursively construct the eight children of this subtree
        glm::vec3 offset;
        float step = halfWidth * 0.5f;
        for (int i = 0; i < 8; i++)
        {
            offset.x = (i & 1) ? step : -step;
            offset.y = (i & 2) ? step : -step;
            offset.z = (i & 4) ? step : -step;
            pNode->children[i] = BuildOctree(center + offset, step, stopDepth - 1);
        }
        return pNode;
    }
}

void OctreeNode::InsertObject(OctreeNode* pTree, Coffee::Entity* pObject)
{
    int index = 0;
    int straddle = 0;

    // Compute the octant number [0..7] the object sphere center is in
    // If straddling any of the dividing x, y or z planes, exit directly
    for (int i = 0; i < 3; i++)
    {
        float delta = pObject->center[i] - pTree->center[i]; // TODO check this
        if (glm::abs(delta) < pTree->halfWidth + pObject->radius)
        {
            straddle = 1;
            break;
        }
        if (delta > 0.0f) index |= (1 << i); // ZYX
    }
    if (!straddle and pTree->children[index])
    {
        // Fully contained in existing child node; insert in that subtree
        InsertObject(pTree->children[index], pObject);
    }
    else
    {
        // Straddling, or no child node to descend into, so
        // link object into linked list at this node
        pObject->pNextObject = pTree->objectList;
        pTree->objectList = pObject;
    }
}

// Test all objects that could possibly overlap due to cell ancestry and coexistence
// in the same cell. Assume objects exist in a single cell only, and fully inside it
void OctreeNode::TestAllCollisions(OctreeNode* pTree)
{
    // Keep track of all ancestor object lists in a stack
    const int MAX_DEPTH = 40;
    static OctreeNode* ancestorStack[MAX_DEPTH];
    static int depth = 0; // 'Depth == 0' is invariant over calls

    // Check collisions between all objects on this level and all
    // ancestor objects. The current level is included as its own
    // ancestor so all necessary pairwise tests are done
    ancestorStack[depth++] = pTree;
    for (int n = 0; n < depth; n++)
    {
        Coffee::Entity* pA, pB;
        for (pA = ancestorStack[n]->objectList; pA; pA = pA->pNextObject)
        {
            for (pB = pTree->objectList; pB; pB = pB->pNextObject)
            {
                // Avoid testing both A->B and B->A
                if (pA == pB) break;
                // Now perform the collision test between pA and pB in some manner
                TestCollision(pA, pB);
            }
        }
    }

    // Recursively visit all existing children
    for (int i = 0; i < 8; i++)
        if (pTree->children[i])
            TestAllCollisions(pTree->children[i]);

    // Remove current node from ancestor stack before returning
    depth--;
}