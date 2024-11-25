#include "Octree.h"

#include "CoffeeEngine/Renderer/DebugRenderer.h"

namespace Coffee {

    void Octree::Insert(OctreeNode& node, const ObjectContainer& object)
    {
        if (!node.aabb.Contains(object.position))
            return;

        if (node.isLeaf) {
            InsertIntoLeaf(node, object);
        } else {
            InsertIntoChild(node, object);
        }
    }

    void Octree::InsertIntoLeaf(OctreeNode& node, const ObjectContainer& object) {
        node.objectList.push_back(object);
        if (node.objectList.size() > maxObjectsPerNode && maxDepth > 0) {
            Subdivide(node);
            RedistributeObjects(node);
        }
    }

    void Octree::InsertIntoChild(OctreeNode& node, const ObjectContainer& object) {
        int childIndex = node.GetChildIndex(node.aabb, object.position);
        Insert(*node.children[childIndex], object);
    }

    void Octree::RedistributeObjects(OctreeNode& node) {
        for (const auto& obj : node.objectList) {
            int childIndex = node.GetChildIndex(node.aabb, obj.position);
            Insert(*node.children[childIndex], obj);
        }
        node.objectList.clear();
    }

    void Octree::Insert(const ObjectContainer& object) {
        Insert(rootNode, object);
    }

    void Octree::Subdivide(OctreeNode& node)
    {
        glm::vec3 center = (node.aabb.min + node.aabb.max) * 0.5f;
        CreateChildren(node, center);
        node.isLeaf = false;
    }

    void Octree::CreateChildren(OctreeNode& node, const glm::vec3& center) {
        node.children[0] = CreateScope<OctreeNode>(AABB(node.aabb.min, center));
        node.children[1] = CreateScope<OctreeNode>(AABB(glm::vec3(center.x, node.aabb.min.y, node.aabb.min.z),
                                                        glm::vec3(node.aabb.max.x, center.y, center.z)));
        node.children[2] = CreateScope<OctreeNode>(AABB(glm::vec3(node.aabb.min.x, center.y, node.aabb.min.z),
                                                        glm::vec3(center.x, node.aabb.max.y, center.z)));
        node.children[3] = CreateScope<OctreeNode>(AABB(glm::vec3(center.x, center.y, node.aabb.min.z),
                                                        glm::vec3(node.aabb.max.x, node.aabb.max.y, center.z)));
        node.children[4] = CreateScope<OctreeNode>(AABB(glm::vec3(node.aabb.min.x, node.aabb.min.y, center.z),
                                                        glm::vec3(center.x, center.y, node.aabb.max.z)));
        node.children[5] = CreateScope<OctreeNode>(AABB(glm::vec3(center.x, node.aabb.min.y, center.z),
                                                        glm::vec3(node.aabb.max.x, center.y, node.aabb.max.z)));
        node.children[6] = CreateScope<OctreeNode>(AABB(glm::vec3(node.aabb.min.x, center.y, center.z),
                                                        glm::vec3(center.x, node.aabb.max.y, node.aabb.max.z)));
        node.children[7] = CreateScope<OctreeNode>(AABB(center, node.aabb.max));
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
                }
            }
        }

        for (auto& obj : objectList)
        {
            DebugRenderer::DrawSphere(obj.position, 0.1f, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
    }

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
        Clear();
    }

    void Octree::DebugDraw()
    {
        rootNode.DebugDrawAABB();
    }

    void Octree::Clear()
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

} // namespace Coffee