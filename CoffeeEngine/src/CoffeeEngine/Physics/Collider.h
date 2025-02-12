#pragma once

#include <glm/vec3.hpp>
#include <Jolt/Jolt.h>

// TODO: Search a better include for JPH::ShapeRefC
#include <Jolt/Physics/PhysicsSystem.h>

namespace Coffee {

    enum class CollisionShapeType
    {
        Box,
        Sphere,
        Capsule,
        Mesh
    };

    struct CollisionShapeConfig
    {
        CollisionShapeType type = CollisionShapeType::Box;
        glm::vec3 size = glm::vec3(1.0f);
        glm::vec3 offset = glm::vec3(0.0f);
    };

    class Collider
    {
        public:
            Collider();
            Collider(const CollisionShapeConfig& shapeConfig, bool isTrigger = false);
            ~Collider() = default;

            const CollisionShapeConfig& GetShapeConfig() const { return m_ShapeConfig; }
            bool IsTrigger() const { return m_IsTrigger; }

            JPH::ShapeRefC GetShape() const { return m_Shape; }
            
        private:
            JPH::ShapeRefC m_Shape;
            CollisionShapeConfig m_ShapeConfig;

            bool m_IsTrigger;
    };

}