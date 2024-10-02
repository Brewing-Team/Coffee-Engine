#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Mesh.h"
namespace Coffee {

    enum class PrimitiveType
    {
        Cube,
        Sphere,
        Plane,
        Quad,
        Cylinder,
        Cone,
        Torus,
        Capsule,
        None
    };

    class PrimitiveMesh
    {
    public:
        template <PrimitiveType T, typename... Args>
        static Ref<Mesh> Create(Args&&... args);

    //private:
        static Ref<Mesh> CreateQuad();

        static Ref<Mesh> CreatePlane(const glm::vec2& size, const glm::vec3& normal = { 0.0f, 1.0f, 0.0f });

        static Ref<Mesh> CreateCube(const glm::vec3& size = { 1.0f, 1.0f, 1.0f });

        static Ref<Mesh> CreateSphere(float radius = 1.0f, uint32_t rings = 32, uint32_t sectors = 32);

        static Ref<Mesh> CreateCylinder(float bottomRadius = 1.0f, float topRadius = 1.0f, float height = 1.0f, int radialSegments = 64, int rings = 8);
    };

    // Explicit specialization declaration
    template <>
    Ref<Mesh> PrimitiveMesh::Create<PrimitiveType::Plane>(const glm::vec2& size, const glm::vec3& normal);

}