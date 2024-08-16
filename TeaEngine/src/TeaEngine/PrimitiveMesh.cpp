#include "PrimitiveMesh.h"
#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Mesh.h"
#include <cstdint>
#include <vector>

namespace Tea {

    template <PrimitiveType T, typename... Args>
    Ref<Mesh> PrimitiveMesh::Create(Args&&... args)
    {
        switch (T)
        {
        case PrimitiveType::Plane:
            return CreatePlane(std::forward<Args>(args)...);
        }
    }

    Ref<Mesh> PrimitiveMesh::CreatePlane(const glm::vec2& size, const glm::vec3& normal)
    {
        std::vector<Vertex> vertices(4);

        vertices[0].Position = { -size.x / 2.0f, 0.0f, -size.y / 2.0f };
        vertices[0].Normals = normal;
        vertices[0].TexCoords = { 0.0f, 0.0f };

        vertices[1].Position = { size.x / 2.0f, 0.0f, -size.y / 2.0f };
        vertices[1].Normals = normal;
        vertices[1].TexCoords = { 1.0f, 0.0f };

        vertices[2].Position = { size.x / 2.0f, 0.0f, size.y / 2.0f };
        vertices[2].Normals = normal;
        vertices[2].TexCoords = { 1.0f, 1.0f };

        vertices[3].Position = { -size.x / 2.0f, 0.0f, size.y / 2.0f };
        vertices[3].Normals = normal;
        vertices[3].TexCoords = { 0.0f, 1.0f };

        std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

        Ref<Mesh> mesh = CreateRef<Mesh>(indices, vertices);

        return mesh;
    }

}