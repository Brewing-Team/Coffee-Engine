#include "PrimitiveMesh.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include <cstdint>
#include <vector>

namespace Coffee {

    template <PrimitiveType T, typename... Args>
    Ref<Mesh> PrimitiveMesh::Create(Args&&... args)
    {
        switch (T)
        {
        case PrimitiveType::Plane:
            return CreatePlane(std::forward<Args>(args)...);
        }
    }

    Ref<Mesh> PrimitiveMesh::CreateQuad()
    {
        std::vector<Vertex> data(4);

        data[0].Position  = glm::vec3(-1.0f, -1.0f, 0.0f);
        data[0].TexCoords = glm::vec2(0.0f, 0.0f);

        data[1].Position  = glm::vec3(1.0f, -1.0f, 0.0f);
        data[1].TexCoords = glm::vec2(1.0f, 0.0f);

        data[2].Position  = glm::vec3(1.0f, 1.0f, 0.0f);
        data[2].TexCoords = glm::vec2(1.0f, 1.0f);

        data[3].Position  = glm::vec3(-1.0f, 1.0f, 0.0f);
        data[3].TexCoords = glm::vec2(0.0f, 1.0f);

        std::vector<uint32_t> indices = {
            0,1,2,2,3,0,
        };

        return CreateRef<Mesh>(indices, data);
    }

    Ref<Mesh> PrimitiveMesh::CreatePlane(const glm::vec2& size, const glm::vec3& normal)
    {
        std::vector<Vertex> vertices(4);

        vertices[0].Position = {-size.x / 2.0f, 0.0f, -size.y / 2.0f};
        vertices[0].Normals = normal;
        vertices[0].TexCoords = {0.0f, 0.0f};

        vertices[1].Position = {size.x / 2.0f, 0.0f, -size.y / 2.0f};
        vertices[1].Normals = normal;
        vertices[1].TexCoords = {1.0f, 0.0f};

        vertices[2].Position = {size.x / 2.0f, 0.0f, size.y / 2.0f};
        vertices[2].Normals = normal;
        vertices[2].TexCoords = {1.0f, 1.0f};

        vertices[3].Position = {-size.x / 2.0f, 0.0f, size.y / 2.0f};
        vertices[3].Normals = normal;
        vertices[3].TexCoords = {0.0f, 1.0f};

        std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};

        Ref<Mesh> mesh = CreateRef<Mesh>(indices, vertices);

        return mesh;
    }

    Ref<Mesh> PrimitiveMesh::CreateCube(const glm::vec3& size)
    {
        std::vector<Vertex> vertices(24);

        vertices[0].Position = {0.5f, 0.5f, 0.5f};
        vertices[0].Normals = {0.0f, 0.0f, 1.0f};

        vertices[1].Position = {-0.5f, 0.5f, 0.5f};
        vertices[1].Normals = {0.0f, 0.0f, 1.0f};

        vertices[2].Position = {-0.5f, -0.5f, 0.5f};
        vertices[2].Normals = {0.0f, 0.0f, 1.0f};

        vertices[3].Position = {0.5f, -0.5f, 0.5f};
        vertices[3].Normals = {0.0f, 0.0f, 1.0f};

        vertices[4].Position = {0.5f, 0.5f, 0.5f};
        vertices[4].Normals = {1.0f, 0.0f, 0.0f};

        vertices[5].Position = {0.5f, -0.5f, 0.5f};
        vertices[5].Normals = {1.0f, 0.0f, 0.0f};

        vertices[6].Position = {0.5f, -0.5f, -0.5f};
        vertices[6].Normals = {1.0f, 0.0f, 0.0f};

        vertices[7].Position = {0.5f, 0.5f, -0.5f};
        vertices[7].Normals = {1.0f, 0.0f, 0.0f};

        vertices[8].Position = {0.5f, 0.5f, 0.5f};
        vertices[8].Normals = {0.0f, 1.0f, 0.0f};

        vertices[9].Position = {0.5f, 0.5f, -0.5f};
        vertices[9].Normals = {0.0f, 1.0f, 0.0f};

        vertices[10].Position = {-0.5f, 0.5f, -0.5f};
        vertices[10].Normals = {0.0f, 1.0f, 0.0f};

        vertices[11].Position = {-0.5f, 0.5f, 0.5f};
        vertices[11].Normals = {0.0f, 1.0f, 0.0f};

        vertices[12].Position = {-0.5f, 0.5f, 0.5f};
        vertices[12].Normals = {-1.0f, 0.0f, 0.0f};

        vertices[13].Position = {-0.5f, 0.5f, -0.5f};
        vertices[13].Normals = {-1.0f, 0.0f, 0.0f};

        vertices[14].Position = {-0.5f, -0.5f, -0.5f};
        vertices[14].Normals = {-1.0f, 0.0f, 0.0f};

        vertices[15].Position = {-0.5f, -0.5f, 0.5f};
        vertices[15].Normals = {-1.0f, 0.0f, 0.0f};

        vertices[16].Position = {-0.5f, -0.5f, -0.5f};
        vertices[16].Normals = {0.0f, -1.0f, 0.0f};

        vertices[17].Position = {0.5f, -0.5f, -0.5f};
        vertices[17].Normals = {0.0f, -1.0f, 0.0f};

        vertices[18].Position = {0.5f, -0.5f, 0.5f};
        vertices[18].Normals = {0.0f, -1.0f, 0.0f};

        vertices[19].Position = {-0.5f, -0.5f, 0.5f};
        vertices[19].Normals = {0.0f, -1.0f, 0.0f};

        vertices[20].Position = {0.5f, -0.5f, -0.5f};
        vertices[20].Normals = {0.0f, 0.0f, -1.0f};

        vertices[21].Position = {-0.5f, -0.5f, -0.5f};
        vertices[21].Normals = {0.0f, 0.0f, -1.0f};

        vertices[22].Position = {-0.5f, 0.5f, -0.5f};
        vertices[22].Normals = {0.0f, 0.0f, -1.0f};

        vertices[23].Position = {0.5f, 0.5f, -0.5f};
        vertices[23].Normals = {0.0f, 0.0f, -1.0f};

        for(int i = 0; i < 6; i++)
        {
            vertices[i * 4 + 0].TexCoords = {0.0f, 0.0f};
            vertices[i * 4 + 1].TexCoords = {1.0f, 0.0f};
            vertices[i * 4 + 2].TexCoords = {1.0f, 1.0f};
            vertices[i * 4 + 3].TexCoords = {0.0f, 1.0f};
        }

        std::vector<uint32_t> indices = {
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7,
            8, 9, 10,
            8, 10, 11,
            12, 13, 14,
            12, 14, 15,
            16, 17, 18,
            16, 18, 19,
            20, 21, 22,
            20, 22, 23
        };

        Ref<Mesh> mesh = CreateRef<Mesh>(indices, vertices);

        return mesh;
    }

} // namespace Coffee