#include "PrimitiveMesh.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include <cstdint>
#include <glm/ext/scalar_constants.hpp>
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

        return CreateRef<Mesh>(indices, vertices);
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

        for (int i = 0; i < 6; i++)
        {
            vertices[i * 4 + 0].TexCoords = {0.0f, 0.0f};
            vertices[i * 4 + 1].TexCoords = {1.0f, 0.0f};
            vertices[i * 4 + 2].TexCoords = {1.0f, 1.0f};
            vertices[i * 4 + 3].TexCoords = {0.0f, 1.0f};
        }

        std::vector<uint32_t> indices = {0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10, 8,  10, 11,
                                         12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23};

        return CreateRef<Mesh>(indices, vertices);;
    }
    Ref<Mesh> PrimitiveMesh::CreateSphere(float radius, uint32_t rings, uint32_t sectors)
    {
        auto data = std::vector<Vertex>{};

        float sectorCount = static_cast<float>(rings);
        float stackCount  = static_cast<float>(sectors);
        float sectorStep  = 2 * glm::pi<float>() / sectorCount;
        float stackStep   = glm::pi<float>() / stackCount;

        for (int i = 0; i <= stackCount; i++)
        {
            float stackAngle = glm::pi<float>() / 2 - i * stackStep;
            float xy         = radius * cosf(stackAngle);
            float z          = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; j++)
            {
                float secotrAngle = j * sectorStep;

                // vertex position (x, y, z)
                float x = xy * cosf(secotrAngle);
                float y = xy * sinf(secotrAngle);

                // vertex tex coord (s, t) range between [0, 1]
                float s = static_cast<float>(j) / sectorCount;
                float t = static_cast<float>(i) / stackCount;

                Vertex vertex;
                vertex.Position = {x, y, z};
                vertex.TexCoords = {s, t};
                vertex.Normals = normalize(vertex.Position); // TODO check this

                data.emplace_back(vertex);
            }
        }

        std::vector<uint32_t> indices;
        uint32_t k1, k2;

        for (uint32_t i = 0; i < stackCount; i++)
        {
            k1 = i * (static_cast<uint32_t>(sectorCount) + 1U); // beginning of current stack
            k2 = k1 + static_cast<uint32_t>(sectorCount) + 1U;  // beginning of next stack

            for (uint32_t j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        return CreateRef<Mesh>(indices, data);

    }

} // namespace Coffee