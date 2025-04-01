#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Math/BoundingBox.h"
#include "CoffeeEngine/Math/Frustum.h"
#include "CoffeeEngine/Renderer/Renderer2D.h"
#include <unordered_map>
#include <vector>

namespace Coffee {

    template <typename T>
    struct ObjectContainer
    {
        const glm::mat4& transform;
        const AABB& aabb;
        const T& object;
        mutable int id;
    };

    template <typename T>
    class OctreeNode
    {
    public:
        AABB aabb;
        bool isLeaf = true;
        int depth = 0; // Depth of the node in the octree
        std::vector<int> objectIDs; // Store only object IDs
        std::array<Scope<OctreeNode>, 8> children;

        void DebugDrawAABB();

    };

    template <typename T>
    class Octree
    {
    public:
        Octree();
        Octree(const AABB& bounds, int maxObjectsPerNode = 8, int maxDepth = 5);
        ~Octree();

        void Insert(Ref<ObjectContainer<T>> object);
        void DebugDraw();
        void Clear();

        std::unordered_map<int, ObjectContainer<T>> Query(const Frustum& frustum) const;


    private:
        void Insert(OctreeNode<T>& node, int objectID);
        void InsertIntoLeaf(OctreeNode<T>& node, int objectID);
        void InsertIntoChild(OctreeNode<T>& node, int objectID);
        void RedistributeObjects(OctreeNode<T>& node);
        void Subdivide(OctreeNode<T>& node);
        void CreateChildren(OctreeNode<T>& node, const glm::vec3& center);

        void Query(const OctreeNode<T>& node, const Frustum& frustum,
                   std::unordered_map<int, ObjectContainer<T>>& results) const;

        OctreeNode<T> rootNode;
        int maxObjectsPerNode;
        int maxDepth;
        int objectsCounter = 0;
    };

    template <typename T>
    void Octree<T>::Insert(Ref<ObjectContainer<T>> object)
    {
        object->id = objectsCounter++;
        objectMap[object->id] = object; // Store in centralized map
        Insert(rootNode, object->id);
    }

    template <typename T>
    void Octree<T>::DebugDraw()
    {
        rootNode.DebugDrawAABB(objectMap);
    }

    template <typename T>
    void Octree<T>::Insert(OctreeNode<T>& node, int objectID)
    {
        AABB objectTransformedAABB = objectMap.at(objectID)->aabb.CalculateTransformedAABB(objectMap.at(objectID)->transform);

        switch (node.aabb.Intersect(objectTransformedAABB))
        {
            using enum IntersectionType;
            case Outside:
                return;
            case Inside:
            case Intersect:
                if (node.isLeaf)
                {
                    InsertIntoLeaf(node, objectID);
                }
                else
                {
                    InsertIntoChild(node, objectID);
                }
        }
    }

    template <typename T>
    void Octree<T>::InsertIntoLeaf(OctreeNode<T>& node, int objectID)
    {
        node.objectIDs.push_back(objectID);
        if (node.objectIDs.size() > maxObjectsPerNode && node.depth < maxDepth)
        {
            Subdivide(node);
            RedistributeObjects(node);
        }
    }

    template <typename T> 
    void Octree<T>::InsertIntoChild(OctreeNode<T>& node, const ObjectContainer<T>& object)
    {
        for (auto& child : node.children)
        {
            if (!child)
                continue;
            if (child->aabb.Intersect(object.aabb.CalculateTransformedAABB(object.transform)) !=
                IntersectionType::Outside)
            {
                Insert(*child, object);
            }
        }

    }

    template <typename T>
    void Octree<T>::RedistributeObjects(OctreeNode<T>& node)
    {
        for (const auto& obj : node.objectList)
        {
            InsertIntoChild(node, obj);
        }
    }

    template <typename T>
    void Octree<T>::Insert(const ObjectContainer<T>& object)
    {
        object.id = objectsCounter;
        objectsCounter++;
        Insert(rootNode, object);
    }

    template <typename T>
    void Octree<T>::Subdivide(OctreeNode<T>& node)
    {
        if (!node.isLeaf)
            return; // If the node is not a leaf, no need to subdivide

        glm::vec3 center = node.aabb.GetCenter(); // Calculate the center of the node's AABB
        CreateChildren(node, center);            // Create child nodes
        node.isLeaf = false;                     // Mark the node as no longer a leaf
    }

    template <typename T>
    void Octree<T>::CreateChildren(OctreeNode<T>& node, const glm::vec3& center)
    {
        node.children[0] = CreateScope<OctreeNode<T>>(AABB(node.aabb.min, center));
        node.children[1] = CreateScope<OctreeNode<T>>(AABB(glm::vec3(center.x, node.aabb.min.y, node.aabb.min.z),
                                                           glm::vec3(node.aabb.max.x, center.y, center.z)));
        node.children[2] = CreateScope<OctreeNode<T>>(AABB(glm::vec3(node.aabb.min.x, center.y, node.aabb.min.z),
                                                           glm::vec3(center.x, node.aabb.max.y, center.z)));
        node.children[3] = CreateScope<OctreeNode<T>>(AABB(glm::vec3(center.x, center.y, node.aabb.min.z),
                                                           glm::vec3(node.aabb.max.x, node.aabb.max.y, center.z)));
        node.children[4] = CreateScope<OctreeNode<T>>(AABB(glm::vec3(node.aabb.min.x, node.aabb.min.y, center.z),
                                                           glm::vec3(center.x, center.y, node.aabb.max.z)));
        node.children[5] = CreateScope<OctreeNode<T>>(AABB(glm::vec3(center.x, node.aabb.min.y, center.z),
                                                           glm::vec3(node.aabb.max.x, center.y, node.aabb.max.z)));
        node.children[6] = CreateScope<OctreeNode<T>>(AABB(glm::vec3(node.aabb.min.x, center.y, center.z),
                                                           glm::vec3(center.x, node.aabb.max.y, node.aabb.max.z)));
        node.children[7] = CreateScope<OctreeNode<T>>(AABB(center, node.aabb.max));
    }

    template <typename T>
    void Octree<T>::Query(const OctreeNode<T>& node, const Frustum& frustum,
                          std::unordered_map<int, ObjectContainer<T>>& results) const
    {
        if (!frustum.Contains(node.aabb))
            return;
    
        for (int id : node.objectIDs)
        {
            if (frustum.Contains(object.aabb.CalculateTransformedAABB(object.transform)))
            {
                results.emplace(object.id, object); // Evita duplicados usando el id como clave
            }
        }
    
        if (node.isLeaf)
            return;
    
        for (const auto& child : node.children)
        {
            if (child)
            {
                Query(*child, frustum, results);
            }
        }
    }

    template <typename T>
    void OctreeNode<T>::DebugDrawAABB()
    {
        // Assuming you have a function to get the number of objects in the node
        int numObjects = objectList.size();

        // Calculate the color based on the number of objects
        float green = glm::clamp(numObjects / 10.0f, 0.0f, 1.0f);
        float red = glm::clamp(1.0f - (numObjects / 10.0f), 0.0f, 1.0f);
        glm::vec4 color(red, green, 0.0f, 1.0f);

        // Draw the box with the calculated color
        Renderer2D::DrawBox(aabb.min, aabb.max, color);
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
            Renderer2D::DrawBox(aabb.min, aabb.max, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        }
    }

    template <typename T>
    Octree<T>::Octree(const AABB& bounds, int maxObjectsPerNode, int maxDepth)
        : maxObjectsPerNode(maxObjectsPerNode), maxDepth(maxDepth)
    {
        rootNode.aabb = bounds;
    }

    template <typename T>
    Octree<T>::~Octree()
    {
        Clear();
    }

    template <typename T>
    void OctreeNode<T>::DebugDrawAABB(const std::unordered_map<int, Ref<ObjectContainer<T>>>& objectMap)
    {
            int numObjects = objectIDs.size();

            float green = glm::clamp(numObjects / 10.0f, 0.0f, 1.0f);
            float red = glm::clamp(1.0f - (numObjects / 10.0f), 0.0f, 1.0f);
            glm::vec4 color(red, green, 0.0f, 1.0f);

            Renderer2D::DrawBox(aabb.min, aabb.max, color);

            for (int id : objectIDs)
            {
                const ObjectContainer<T>& obj = objectMap.at(id);
                AABB aabb = obj.aabb.CalculateTransformedAABB(obj.transform);
                Renderer2D::DrawBox(aabb.min, aabb.max, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
            }

            for (const auto& child : children)
            {
                if (child)
                {
                    child->DebugDrawAABB(objectMap);
                }
            }
    }

    template <typename T>
    void Octree<T>::Clear()
    {
        rootNode.objectIDs.clear();
        for (auto& child : rootNode.children)
        {
            if (child)
            {
                child->objectIDs.clear();
                child.reset();
            }
        }
        rootNode.isLeaf = true;
        objectMap.clear(); // Clear centralized storage
    }

    template <typename T>
    std::unordered_map<int, ObjectContainer<T>> Octree<T>::Query(const Frustum& frustum) const
    {
        std::unordered_map<int, ObjectContainer<T>> results;
        Query(rootNode, frustum, results);
        return results;
    }

} // namespace Coffee