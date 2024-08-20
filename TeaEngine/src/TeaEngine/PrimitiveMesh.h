#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Mesh.h"
namespace Tea {

    enum class PrimitiveType
    {
        //Cube,
        //Sphere,
        Plane
    };

    class PrimitiveMesh
    {
    public:
        template <PrimitiveType T, typename... Args>
        static Ref<Mesh> Create(Args&&... args);

    //private:
        static Ref<Mesh> CreateQuad();

        static Ref<Mesh> CreatePlane(const glm::vec2& size, const glm::vec3& normal = { 0.0f, 1.0f, 0.0f });
    };

    // Explicit specialization declaration
    template <>
    Ref<Mesh> PrimitiveMesh::Create<PrimitiveType::Plane>(const glm::vec2& size, const glm::vec3& normal);

}