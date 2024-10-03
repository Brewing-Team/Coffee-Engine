#include "PrimitiveMesh.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include <cstdint>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/constants.hpp>
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

        return CreateRef<Mesh>(indices, vertices);
        ;
    }

    Ref<Mesh> PrimitiveMesh::CreateSphere(float radius, float height, int radialSegments, int rings, bool isHemiSphere)
    {

        int i, j, prevrow, thisrow, point;
        float x, y, z;

        std::vector<Vertex> data;
        std::vector<uint32_t> indices;

        float scale = height * (isHemiSphere ? 1.0 : 0.5);

        thisrow = 0;
        prevrow = 0;
        for (j = 0; j <= (rings + 1); j++)
        {
            float v = j;
            float w;

            v /= (rings + 1);
            w = glm::sin(glm::pi<float>() * v);
            y = scale * cos(glm::pi<float>() * v);

            for(i = 0; i <= radialSegments; i++)
            {
                float u = i;
                u /= radialSegments;

                x = sin(u * glm::tau<float>());
                z = cos(u * glm::tau<float>());

                Vertex vertex;
                if (isHemiSphere && y < 0.0f) {
                    glm::vec3 p = glm::vec3(x * radius * w, 0.0f, z * radius * w);
                    vertex.Position = p;
                    vertex.Normals = glm::vec3(0.0f, -1.0f, 0.0f);
                    vertex.Tangent = glm::vec4(z, 0.0f, -x, 1.0f);
                    vertex.TexCoords = glm::vec2(u, v);
                    data.emplace_back(vertex);
                } else {
                    glm::vec3 p = glm::vec3(x * radius * w, y, z * radius * w);
                    glm::vec3 normal = glm::vec3(x * w * scale, radius * (y / scale), z * w * scale);

                    vertex.Position = p;
                    vertex.Normals = glm::normalize(normal);
                    vertex.Tangent = glm::vec4(z, 0.0f, -x, 1.0f);
                    vertex.TexCoords = glm::vec2(u, v);
                    data.emplace_back(vertex);
                }

                point++;

                if (i > 0 && j > 0) {
                    indices.push_back(prevrow + i - 1);
                    indices.push_back(prevrow + i);
                    indices.push_back(thisrow + i - 1);

                    indices.push_back(prevrow + i);
                    indices.push_back(thisrow + i);
                    indices.push_back(thisrow + i - 1);
                }
            }

            prevrow = thisrow;
            thisrow = point;
        }

        return CreateRef<Mesh>(indices, data);
    }

    Ref<Mesh> PrimitiveMesh::CreateCylinder(float bottomRadius, float topRadius, float height, int radialSegments,
                                            int rings)
    {
        int i, j, prevrow, thisrow, point = 0;
        float x, y, z, u, v, radius;

        std::vector<Vertex> data;
        std::vector<uint32_t> indices;

        thisrow = 0;
        prevrow = 0;

        for (j = 0; j <= (rings + 1); j++)
        {
            v = static_cast<float>(j) / (rings + 1);
            radius = topRadius + ((bottomRadius - topRadius) * v);
            y = height * v - (height * 0.5f);

            for (i = 0; i <= radialSegments; i++)
            {
                u = static_cast<float>(i) / radialSegments;
                x = sin(u * (glm::pi<float>() * 2.0f));
                z = cos(u * (glm::pi<float>() * 2.0f));

                glm::vec3 p = glm::vec3(x * radius, y, z * radius);

                Vertex vertex;
                vertex.Position = p;
                vertex.Normals = glm::vec3(x, 0.0f, z);
                vertex.TexCoords = glm::vec2(u, v * 0.5f);
                data.emplace_back(vertex);

                point++;

                if (i > 0 && j > 0)
                {
                    indices.push_back(thisrow + i - 1);
                    indices.push_back(prevrow + i);
                    indices.push_back(prevrow + i - 1);

                    indices.push_back(thisrow + i - 1);
                    indices.push_back(thisrow + i);
                    indices.push_back(prevrow + i);
                }
            }

            prevrow = thisrow;
            thisrow = point;
        }

        // Add top cap
        if (topRadius > 0.0f)
        {
            y = height * 0.5f;

            Vertex vertex;
            vertex.Position = glm::vec3(0.0f, y, 0.0f);
            vertex.Normals = glm::vec3(0.0f, 1.0f, 0.0f);
            vertex.TexCoords = glm::vec2(0.25f, 0.75f);
            data.emplace_back(vertex);
            point++;

            for (i = 0; i <= radialSegments; i++)
            {
                float r = static_cast<float>(i) / radialSegments;
                x = sin(r * (glm::pi<float>() * 2.0f));
                z = cos(r * (glm::pi<float>() * 2.0f));

                u = (x + 1.0f) * 0.25f;
                v = 0.5f + (z * 0.25f);

                glm::vec3 p = glm::vec3(x * topRadius, y, z * topRadius);
                Vertex vertex;
                vertex.Position = p;
                vertex.Normals = glm::vec3(0.0f, 1.0f, 0.0f);
                vertex.TexCoords = glm::vec2(u, v);
                data.emplace_back(vertex);
                point++;

                if (i > 0)
                {
                    indices.push_back(point - 2);
                    indices.push_back(point - 1);
                    indices.push_back(thisrow);
                }
            }
        }

        // Add bottom cap
        if (bottomRadius > 0.0f)
        {
            y = height * -0.5f;

            thisrow = point;

            Vertex vertex;
            vertex.Position = glm::vec3(0.0f, y, 0.0f);
            vertex.Normals = glm::vec3(0.0f, -1.0f, 0.0f);
            vertex.TexCoords = glm::vec2(0.75f, 0.75f);
            data.emplace_back(vertex);
            point++;

            for (i = 0; i <= radialSegments; i++)
            {
                float r = static_cast<float>(i) / radialSegments;
                x = sin(r * (glm::pi<float>() * 2.0f));
                z = cos(r * (glm::pi<float>() * 2.0f));

                u = 0.5f + (x * 0.25f);
                v = 1.0f + (z * 0.25f);

                glm::vec3 p = glm::vec3(x * bottomRadius, y, z * bottomRadius);

                vertex.Position = p;
                vertex.Normals = glm::vec3(0.0f, -1.0f, 0.0f);
                vertex.TexCoords = glm::vec2(u, v);
                data.emplace_back(vertex);
                point++;

                if (i > 0)
                {
                    indices.push_back(point - 1);
                    indices.push_back(point - 2);
                    indices.push_back(thisrow);
                }
            }
        }

        return CreateRef<Mesh>(indices, data);
    }

    Ref<Mesh> PrimitiveMesh::CreateTorus(float innerRadius, float outerRadius, int rings, int ringSegments)
    {
        std::vector<uint32_t> indices;
        auto data = std::vector<Vertex>{};
        float minRadius = innerRadius;
        float maxRadius = outerRadius;

        if (minRadius > maxRadius)
        {
            std::swap(minRadius, maxRadius);
        }

        float radius = (maxRadius - minRadius) * 0.5f;

        float horizontalTotal = maxRadius * glm::tau<float>(); // ???
        float maxH = maxRadius * glm::tau<float>() / horizontalTotal;
        float deltaH = (maxRadius - minRadius) * glm::tau<float>() / horizontalTotal;
        float heightV = radius * glm::tau<float>() / (radius * glm::tau<float>());

        for (int i = 0; i <= rings; i++)
        {
            int prevrow = (i - 1) * (ringSegments + 1);
            int thisrow = i * (ringSegments + 1);
            float inci = float(i) / rings;
            float angi = inci * glm::tau<float>();

            glm::vec2 normali = glm::vec2(-sin(angi), -cos(angi));

            for (int j = 0; j <= ringSegments; j++)
            {
                float incj = float(j) / ringSegments;
                float angj = incj * glm::tau<float>();

                glm::vec2 normalj = glm::vec2(cos(angj), sin(angj));
                glm::vec2 normalk = normalj * radius + glm::vec2(minRadius, 0.0f);

                float offsetH = 0.5f * (1.0f - normalj.x) * deltaH;
                float adjH = maxH - offsetH;
                offsetH *= 0.5f;

                Vertex vertex;
                vertex.Position = glm::vec3(normali.x * normalk.x, normalk.y, normali.y * normalk.x);
                vertex.Normals = glm::vec3(normali.x * normalj.x, normalj.y, normali.y * normalj.x);
                vertex.Tangent = glm::vec4(-cos(angi), 0.0f, sin(angi), 1.0f);
                vertex.TexCoords = glm::vec2(inci, incj);
                data.emplace_back(vertex);

                if (i > 0 && j > 0)
                {
                    indices.push_back(thisrow + j - 1);
                    indices.push_back(prevrow + j);
                    indices.push_back(prevrow + j - 1);

                    indices.push_back(thisrow + j - 1);
                    indices.push_back(thisrow + j);
                    indices.push_back(prevrow + j);
                }
            }
        }

        return CreateRef<Mesh>(indices, data);
    }

    Ref<Mesh> PrimitiveMesh::CreateCapsule(float radius, float height, int radialSegments, int rings)
    {
        std::vector<uint32_t> indices;
        auto data = std::vector<Vertex>{};
        int i, j, prevrow, thisrow, point;
        float x, y, z, u, v, w;
        float onethird = 1.0f / 3.0f;
        float twothirds = 2.0f / 3.0f;

        // top hemisphere
        thisrow = 0;
        prevrow = 0;
        for (j = 0; j <= (rings + 1); j++)
        {
            v = j;
            v /= rings + 1;
            w = sin(0.5 * glm::pi<float>() * v);
            y = radius * cos(0.5 * glm::pi<float>() * v);

            for (i = 0; i <= radialSegments; i++)
            {
                u = i;
                u /= radialSegments;

                x = -sin(u * glm::tau<float>());
                z = cos(u * glm::tau<float>());

                glm::vec3 p = glm::vec3(x * radius * w, y, -z * radius * w);
                Vertex vertex;
                vertex.Position = p + glm::vec3(0.0f, 0.5f * height - radius, 0.0f),
                vertex.Normals = glm::normalize(p);
                vertex.Tangent = glm::vec4(-z, 0.0f, -x, 1.0f);
                vertex.TexCoords = glm::vec2(u, v * onethird); // TODO check this
                data.emplace_back(vertex);

                point++;

                if (i > 0 && j > 0)
                {
                    indices.push_back(prevrow + i - 1);
                    indices.push_back(prevrow + i);
                    indices.push_back(thisrow + i - 1);

                    indices.push_back(prevrow + i);
                    indices.push_back(thisrow + i);
                    indices.push_back(thisrow + i - 1);
                }
            }

            prevrow = thisrow;
            thisrow = point;
        }

        // cylinder
        thisrow = point;
        prevrow = 0;
        for (j = 0; j <= (rings + 1); j++)
        {
            v = j;
            v /= rings + 1;

            y = (height - 2.0 * radius) * v;
            y = (0.5 * height - radius) - y;

            for (i = 0; i <= radialSegments; i++)
            {
                u = i;
                u /= radialSegments;

                x = -sin(u * glm::tau<float>());
                z = cos(u * glm::tau<float>());

                Vertex vertex;
                vertex.Position = glm::vec3(x * radius, y, -z * radius);
                vertex.Normals = glm::normalize(glm::vec3(x, 0.0f, -z));
                vertex.Tangent = glm::vec4(-z, 0.0f, -x, 1.0f);
                vertex.TexCoords = glm::vec2(u, onethird + (v * onethird)); // TODO check this
                data.emplace_back(vertex);

                point++;

                if (i > 0 && j > 0)
                {
                    indices.push_back(prevrow + i - 1);
                    indices.push_back(prevrow + i);
                    indices.push_back(thisrow + i - 1);

                    indices.push_back(prevrow + i);
                    indices.push_back(thisrow + i);
                    indices.push_back(thisrow + i - 1);
                }
            }

            prevrow = thisrow;
            thisrow = point;
        }

        // bottom hemisphere
        thisrow = point;
        prevrow = 0;
        for (j = 0; j <= (rings + 1); j++)
        {
            v = j;

            v /= (rings + 1);
            v += 1.0;
            w = sin(0.5 * glm::pi<float>() * v);
            y = radius * glm::cos(0.5 * glm::pi<float>() * v);

            for (i = 0; i <= radialSegments; i++)
            {
                u = i;
                u /= radialSegments;

                x = -glm::sin(u * glm::tau<float>());
                z = glm::cos(u * glm::tau<float>());

                Vertex vertex;
                glm::vec3 p = glm::vec3(x * radius * w, y, -z * radius * w);
                vertex.Position = p + glm::vec3(0.0f, -0.5f * height + radius, 0.0f);
                vertex.Normals = glm::normalize(p);
                vertex.Tangent = glm::vec4(-z, 0.0f, -x, 1.0f);
                vertex.TexCoords = glm::vec2(u, twothirds + ((v - 1.0f) * onethird));
                data.emplace_back(vertex);

                point++;

                if (i > 0 && j > 0)
                {
                    indices.push_back(prevrow + i - 1);
                    indices.push_back(prevrow + i);
                    indices.push_back(thisrow + i - 1);

                    indices.push_back(prevrow + i);
                    indices.push_back(thisrow + i);
                    indices.push_back(thisrow + i - 1);
                }
            }

            prevrow = thisrow;
            thisrow = point;
        }

        return CreateRef<Mesh>(indices, data);
    }

} // namespace Coffee