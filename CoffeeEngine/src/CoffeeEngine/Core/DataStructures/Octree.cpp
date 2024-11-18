#include "Octree.h"

#include "CoffeeEngine/Renderer/DebugRenderer.h"
#include "CoffeeEngine/Scene/Components.h"

namespace Coffee
{
    OctreeNode::OctreeNode(OctreeNode* parent, AABB aabb) : parent(parent), aabb(aabb) {}

    OctreeNode::~OctreeNode()
    {
        // Free all the children
        if (depth == 0)
            return;

        for (auto & i : children)
        {
            delete i;
        }
    }

    void OctreeNode::Insert(ObjectContainer object)
    {
        if (objectList.size() < maxObjectsPerNode)
        {
            Insert(object);
        }
        else
        {
            Subdivide(depth--);
            for (auto& object : objectList)
            {
                Insert(object);
            }
        }
    }

    void OctreeNode::Subdivide(int depth)
    {
        this->depth = depth;

        if (depth == 0)
            return;

        glm::vec3 halfSize = (aabb.max - aabb.min) * 0.5f;
        glm::vec3 center = (aabb.max + aabb.min) * 0.5f;

        for (int i = 0; i < 8; ++i)
        {
            glm::vec3 newMin = center;
            glm::vec3 newMax = center;

            newMin.x += (i & 1 ? 0.0f : -halfSize.x);
            newMax.x += (i & 1 ? halfSize.x : 0.0f);

            newMin.y += (i & 2 ? 0.0f : -halfSize.y);
            newMax.y += (i & 2 ? halfSize.y : 0.0f);

            newMin.z += (i & 4 ? 0.0f : -halfSize.z);
            newMax.z += (i & 4 ? halfSize.z : 0.0f);

            AABB newAABB(newMin, newMax);
            children[i] = new OctreeNode(this, newAABB);
        }
    }

    void OctreeNode::DebugDrawAABB()
    {
        // Calculate hue based on depth
        float hue = static_cast<float>(depth) / 10.0f;            // Adjust the divisor to control the hue range
        glm::vec4 color = glm::vec4(hue, 1.0f - hue, 0.0f, 1.0f); // Create a color based on the hue

        DebugRenderer::DrawBox(aabb, color, 1.0f);

        if (depth == 0)
            return;

        for (auto& child : children)
        {
            child->DebugDrawAABB();
        }
    }

    Octree::Octree(AABB bounds)
    {
        rootNode = new OctreeNode(nullptr, bounds);
    }

    Octree::~Octree()
    {
        delete rootNode;
    }

    void Octree::Insert(const glm::vec3& position, glm::vec3 data)
    {
        // if this node has space for more objects, insert it
        // else subdivide the node and insert all the objects in the new nodes (check the aabb and insert in all the nodes that the object is colliding with)

        if (rootNode->objectList.size() < rootNode->maxObjectsPerNode)
        {
            rootNode->Insert({ position, data });
        }
        else
        {
            rootNode->Subdivide(maxDepth--);
            for (auto& object : rootNode->objectList)
            {
                rootNode->Insert(object);
            }
        }
    }

    void Octree::Update()
    {
        // Draw parent
        rootNode->DebugDrawAABB();

        // Update the nodes, so that the objects are in the correct nodes
        // TODO
    }

} // namespace Coffee