#include "Octree.h"

#include "CoffeeEngine/Renderer/DebugRenderer.h"
#include "CoffeeEngine/Scene/Components.h"

namespace Coffee {

    void Octree::Insert(OctreeNode& node, const ObjectContainer& object)
    {
        // If the object is not inside the node, return
        if (!node.aabb.Contains(object.position))
            return;

        if (node.isLeaf) // If the node is a leaf, insert the object into the node
        {
            node.objectList.push_back(object);
            if (node.objectList.size() > maxObjectsPerNode and maxDepth > 0)
            {
                // If the node is full, subdivide it and distribute the objects to the children
                Subdivide(node);
                for (const auto& obj : node.objectList)
                {
                    int childIndex = node.GetChildIndex(node.aabb, object.position);
                    Insert(*node.children[childIndex], obj);
                }
                node.objectList.clear();
            }
        } else { // If the node is not a leaf, insert the object into the correct child
            int childIndex  = node.GetChildIndex(node.aabb, object.position);
            Insert(*node.children[childIndex], object);
        }
    }

    void Octree::Insert(const ObjectContainer& object) {
        Insert(rootNode, object);
    }

    void Octree::Subdivide(OctreeNode& node)
    {
        glm::vec3 center = (node.aabb.min + node.aabb.max) * 0.5f;
        glm::vec3 halfSize = (node.aabb.max - node.aabb.min) * 0.5f; // delete this

        // Create the 8 children nodes of the current node
        node.children[0] = std::make_unique<OctreeNode>(AABB(node.aabb.min, center));
        node.children[1] = std::make_unique<OctreeNode>(AABB(glm::vec3(center.x, node.aabb.min.y, node.aabb.min.z),
                                                             glm::vec3(node.aabb.max.x, center.y, center.z)));
        node.children[2] = std::make_unique<OctreeNode>(AABB(glm::vec3(node.aabb.min.x, center.y, node.aabb.min.z),
                                                             glm::vec3(center.x, node.aabb.max.y, center.z)));
        node.children[3] = std::make_unique<OctreeNode>(AABB(glm::vec3(center.x, center.y, node.aabb.min.z),
                                                             glm::vec3(node.aabb.max.x, node.aabb.max.y, center.z)));
        node.children[4] = std::make_unique<OctreeNode>(AABB(glm::vec3(node.aabb.min.x, node.aabb.min.y, center.z),
                                                             glm::vec3(center.x, center.y, node.aabb.max.z)));
        node.children[5] = std::make_unique<OctreeNode>(AABB(glm::vec3(center.x, node.aabb.min.y, center.z),
                                                             glm::vec3(node.aabb.max.x, center.y, node.aabb.max.z)));
        node.children[6] = std::make_unique<OctreeNode>(AABB(glm::vec3(node.aabb.min.x, center.y, center.z),
                                                             glm::vec3(center.x, node.aabb.max.y, node.aabb.max.z)));
        node.children[7] = std::make_unique<OctreeNode>(AABB(center, node.aabb.max));

        // Set the node as not a leaf
        node.isLeaf = false;
    }

    void OctreeNode::DebugDrawAABB()
    {
        DebugRenderer::DrawBox(aabb.min, aabb.max, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        if (!isLeaf)
        {
            for (auto& child : children)
            {
                if (child)
                {
                    child->DebugDrawAABB();
                    for (auto& obj : child->objectList)
                    {
                        DebugRenderer::DrawSphere(obj.position, 0.1f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
                    }
                }
            }
        }
    }

    // Get the index of the child node that contains the point
    int OctreeNode::GetChildIndex(const AABB& bounds, const glm::vec3& point) const
    {
        glm::vec3 center = (bounds.min + bounds.max) * 0.5f;
        int index = 0;
        if (point.x > center.x) index |= 1;
        if (point.y > center.y) index |= 2;
        if (point.z > center.z) index |= 4;
        return index;
    }

    Octree::Octree(const AABB& bounds, int maxObjectsPerNode, int maxDepth) : maxObjectsPerNode(maxObjectsPerNode), maxDepth(maxDepth)
    {
        rootNode.aabb = bounds;
    }

    Octree::~Octree()
    {
        rootNode.objectList.clear();
        for (auto& child : rootNode.children) {
            if (child) {
                child->objectList.clear();
                child.reset();
            }
        }
        rootNode.isLeaf = true;
    }

    void Octree::Update()
    {
        // Draw parent
        rootNode.DebugDrawAABB();

        // Update the nodes, so that the objects are in the correct nodes
        // TODO
    }

} // namespace Coffee