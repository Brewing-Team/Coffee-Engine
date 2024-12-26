#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Renderer/DebugRenderer.h"
#include <vector>
#include <memory>

namespace Coffee {

    template <typename T>
    struct ObjectContainer
    {
        glm::mat4& transform;
        AABB& aabb;
        T& object;
    };

    template <typename T>
    class OctreeNode
    {
    public:
        AABB localAABB;
        glm::mat4 transform;
        bool isLeaf = true;
        std::vector<ObjectContainer<T>> objectList;
        std::array<Scope<OctreeNode>, 8> children;

        void DebugDrawAABB();
        int GetChildIndex(const AABB& bounds, const glm::vec3& point) const;
    };

    template <typename T>
    class Octree
    {
    public:
        Octree();
        Octree(const AABB& bounds, int maxObjectsPerNode = 8, int maxDepth = 5);
        ~Octree();

        void Insert(const ObjectContainer<T>& object);
        void DebugDraw();
        void Clear();

    private:
        void Insert(OctreeNode<T>& node, const ObjectContainer<T>& object);
        void InsertIntoLeaf(OctreeNode<T>& node, const ObjectContainer<T>& object);
        void InsertIntoChild(OctreeNode<T>& node, const ObjectContainer<T>& object);
        void RedistributeObjects(OctreeNode<T>& node);
        void Subdivide(OctreeNode<T>& node);
        void CreateChildren(OctreeNode<T>& node, const glm::vec3& center);

        OctreeNode<T> rootNode;
        int maxObjectsPerNode;
        int maxDepth;
    };

    template <typename T>
    void Octree<T>::Insert(OctreeNode<T>& node, const ObjectContainer<T>& object)
    {
        if (node.localAABB.Intersect(object.position) != IntersectionType::Inside)
            return;

        if (node.isLeaf) {
            InsertIntoLeaf(node, object);
        } else {
            InsertIntoChild(node, object);
        }
    }

    template <typename T>
    void Octree<T>::InsertIntoLeaf(OctreeNode<T>& node, const ObjectContainer<T>& object) {
        node.objectList.push_back(object);
        if (node.objectList.size() > maxObjectsPerNode && maxDepth > 0) {
            Subdivide(node);
            RedistributeObjects(node);
        }
    }

    template <typename T>
    void Octree<T>::InsertIntoChild(OctreeNode<T>& node, const ObjectContainer<T>& object) {
        int childIndex = node.GetChildIndex(node.localAABB, object.position);
        Insert(*node.children[childIndex], object);
    }

    template <typename T>
    void Octree<T>::RedistributeObjects(OctreeNode<T>& node) {
        for (const auto& obj : node.objectList) {
            int childIndex = node.GetChildIndex(node.localAABB, obj.position);
            Insert(*node.children[childIndex], obj);
        }
        node.objectList.clear();
    }

    template <typename T>
    void Octree<T>::Insert(const ObjectContainer<T>& object) {
        Insert(rootNode, object);
    }

    template <typename T>
    void Octree<T>::Subdivide(OctreeNode<T>& node)
    {
        glm::vec3 center = (node.localAABB.min + node.localAABB.max) * 0.5f;
        CreateChildren(node, center);
        node.isLeaf = false;
    }

    template <typename T>
    void Octree<T>::CreateChildren(OctreeNode<T>& node, const glm::vec3& center) {
        node.children[0] = CreateScope<OctreeNode<T>>(AABB(node.localAABB.min, center));
        node.children[1] = CreateScope<OctreeNode<T>>(AABB(glm::vec3(center.x, node.localAABB.min.y, node.localAABB.min.z),
                                                        glm::vec3(node.localAABB.max.x, center.y, center.z)));
        node.children[2] = CreateScope<OctreeNode<T>>(AABB(glm::vec3(node.localAABB.min.x, center.y, node.localAABB.min.z),
                                                        glm::vec3(center.x, node.localAABB.max.y, center.z)));
        node.children[3] = CreateScope<OctreeNode<T>>(AABB(glm::vec3(center.x, center.y, node.localAABB.min.z),
                                                        glm::vec3(node.localAABB.max.x, node.localAABB.max.y, center.z)));
        node.children[4] = CreateScope<OctreeNode<T>>(AABB(glm::vec3(node.localAABB.min.x, node.localAABB.min.y, center.z),
                                                        glm::vec3(center.x, center.y, node.localAABB.max.z)));
        node.children[5] = CreateScope<OctreeNode<T>>(AABB(glm::vec3(center.x, node.localAABB.min.y, center.z),
                                                        glm::vec3(node.localAABB.max.x, center.y, node.localAABB.max.z)));
        node.children[6] = CreateScope<OctreeNode<T>>(AABB(glm::vec3(node.localAABB.min.x, center.y, center.z),
                                                        glm::vec3(center.x, node.localAABB.max.y, node.localAABB.max.z)));
        node.children[7] = CreateScope<OctreeNode<T>>(AABB(center, node.localAABB.max));
    }

    template <typename T>
    void OctreeNode<T>::DebugDrawAABB()
    {
        AABB transformedAABB = localAABB.Transform(transform);
        DebugRenderer::DrawBox(transformedAABB.min, transformedAABB.max, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
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
            AABB aabb = obj.aabb.CalculateTransformedAABB(obj.transform);
            DebugRenderer::DrawBox(aabb.min, aabb.max, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
    }

    template <typename T>
    int OctreeNode<T>::GetChildIndex(const AABB& bounds, const glm::vec3& point) const
    {
        glm::vec3 center = (bounds.min + bounds.max) * 0.5f;
        int index = 0;
        if (point.x > center.x) index |= 1;
        if (point.y > center.y) index |= 2;
        if (point.z > center.z) index |= 4;
        return index;
    }

    template <typename T>
    Octree<T>::Octree(const AABB& bounds, int maxObjectsPerNode, int maxDepth) : maxObjectsPerNode(maxObjectsPerNode), maxDepth(maxDepth)
    {
        rootNode.localAABB = bounds;
        rootNode.transform = glm::mat4(1.0f);
    }

    template <typename T>
    Octree<T>::~Octree()
    {
        Clear();
    }

    template <typename T>
    void Octree<T>::DebugDraw()
    {
        rootNode.DebugDrawAABB();
    }

    template <typename T>
    void Octree<T>::Clear()
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