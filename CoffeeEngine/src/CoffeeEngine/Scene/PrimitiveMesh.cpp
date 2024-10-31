#include "PrimitiveMesh.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/Renderer/Mesh.h"
#include <cstdint>
#include <glm/gtc/constants.hpp>
#include <vector>

namespace Coffee {

    /*
     *
     * FIXME - This function is currently not implemented.
     *
    template <PrimitiveType T, typename... Args>
    Ref<Mesh> PrimitiveMesh::Create(Args&&... args)
    {
        switch (T)
        {
        case PrimitiveType::Plane:
            return CreatePlane(std::forward<Args>(args)...);
        }
    }
    */

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

        const Ref<Mesh>& quadMesh = CreateRef<Mesh>(indices, data);
        quadMesh->SetName("Quad");

        AABB quadAABB(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        quadMesh->SetAABB(quadAABB);

        return quadMesh;
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

        const Ref<Mesh>& planeMesh = CreateRef<Mesh>(indices, vertices);
        planeMesh->SetName("Plane");

        AABB planeAABB(glm::vec3(-size.x * 0.5f, 0.0f, -size.y * 0.5f), glm::vec3(size.x * 0.5f, 0.0f, size.y * 0.5f));
        planeMesh->SetAABB(planeAABB);

        return planeMesh;
    }

    /**
     * @brief Creates a cube mesh.
     * 
     * @param size The size of the cube.
     * @param subdivideW Subdivision parameter for width (currently not functional).
     * @param subdivideH Subdivision parameter for height (currently not functional).
     * @param subdivideD Subdivision parameter for depth (currently not functional).
     * @return Ref<Mesh> A reference to the created cube mesh.
     */
    Ref<Mesh> PrimitiveMesh::CreateCube(const glm::vec3& size, int subdivideW, int subdivideH, int subdivideD)
    {
        std::vector<Vertex> vertices(24);

        // Front
        vertices[0].Position = {-size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f};
        vertices[0].Normals = {0.0f, 0.0f, 1.0f};
        vertices[0].TexCoords = {0.0f, 0.0f};

        vertices[1].Position = {size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f};
        vertices[1].Normals = {0.0f, 0.0f, 1.0f};
        vertices[1].TexCoords = {1.0f, 0.0f};

        vertices[2].Position = {size.x / 2.0f, size.y / 2.0f, size.z / 2.0f};
        vertices[2].Normals = {0.0f, 0.0f, 1.0f};
        vertices[2].TexCoords = {1.0f, 1.0f};

        vertices[3].Position = {-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f};
        vertices[3].Normals = {0.0f, 0.0f, 1.0f};
        vertices[3].TexCoords = {0.0f, 1.0f};

        // Back
        vertices[4].Position = {size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f};
        vertices[4].Normals = {0.0f, 0.0f, -1.0f};
        vertices[4].TexCoords = {0.0f, 0.0f};

        vertices[5].Position = {-size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f};
        vertices[5].Normals = {0.0f, 0.0f, -1.0f};
        vertices[5].TexCoords = {1.0f, 0.0f};

        vertices[6].Position = {-size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f};
        vertices[6].Normals = {0.0f, 0.0f, -1.0f};
        vertices[6].TexCoords = {1.0f, 1.0f};

        vertices[7].Position = {size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f};
        vertices[7].Normals = {0.0f, 0.0f, -1.0f};
        vertices[7].TexCoords = {0.0f, 1.0f};

        // Top

        vertices[8].Position = {-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f};
        vertices[8].Normals = {0.0f, 1.0f, 0.0f};
        vertices[8].TexCoords = {0.0f, 0.0f};

        vertices[9].Position = {size.x / 2.0f, size.y / 2.0f, size.z / 2.0f};
        vertices[9].Normals = {0.0f, 1.0f, 0.0f};
        vertices[9].TexCoords = {1.0f, 0.0f};

        vertices[10].Position = {size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f};
        vertices[10].Normals = {0.0f, 1.0f, 0.0f};
        vertices[10].TexCoords = {1.0f, 1.0f};

        vertices[11].Position = {-size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f};
        vertices[11].Normals = {0.0f, 1.0f, 0.0f};
        vertices[11].TexCoords = {0.0f, 1.0f};

        // Bottom
        vertices[12].Position = {size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f};
        vertices[12].Normals = {0.0f, -1.0f, 0.0f};
        vertices[12].TexCoords = {0.0f, 0.0f};

        vertices[13].Position = {-size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f};
        vertices[13].Normals = {0.0f, -1.0f, 0.0f};
        vertices[13].TexCoords = {1.0f, 0.0f};

        vertices[14].Position = {-size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f};
        vertices[14].Normals = {0.0f, -1.0f, 0.0f};
        vertices[14].TexCoords = {1.0f, 1.0f};

        vertices[15].Position = {size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f};
        vertices[15].Normals = {0.0f, -1.0f, 0.0f};
        vertices[15].TexCoords = {0.0f, 1.0f};

        // Right
        vertices[16].Position = {size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f};
        vertices[16].Normals = {1.0f, 0.0f, 0.0f};
        vertices[16].TexCoords = {0.0f, 0.0f};

        vertices[17].Position = {size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f};
        vertices[17].Normals = {1.0f, 0.0f, 0.0f};
        vertices[17].TexCoords = {1.0f, 0.0f};

        vertices[18].Position = {size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f};
        vertices[18].Normals = {1.0f, 0.0f, 0.0f};
        vertices[18].TexCoords = {1.0f, 1.0f};

        vertices[19].Position = {size.x / 2.0f, size.y / 2.0f, size.z / 2.0f};
        vertices[19].Normals = {1.0f, 0.0f, 0.0f};
        vertices[19].TexCoords = {0.0f, 1.0f};

        // Left
        vertices[20].Position = {-size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f};
        vertices[20].Normals = {-1.0f, 0.0f, 0.0f};
        vertices[20].TexCoords = {0.0f, 0.0f};

        vertices[21].Position = {-size.x / 2.0f, -size.y / 2.0f, size.z / 2.0f};
        vertices[21].Normals = {-1.0f, 0.0f, 0.0f};
        vertices[21].TexCoords = {1.0f, 0.0f};

        vertices[22].Position = {-size.x / 2.0f, size.y / 2.0f, size.z / 2.0f};
        vertices[22].Normals = {-1.0f, 0.0f, 0.0f};
        vertices[22].TexCoords = {1.0f, 1.0f};

        vertices[23].Position = {-size.x / 2.0f, size.y / 2.0f, -size.z / 2.0f};
        vertices[23].Normals = {-1.0f, 0.0f, 0.0f};
        vertices[23].TexCoords = {0.0f, 1.0f};

        std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0, // Front
            4, 5, 6, 6, 7, 4, // Back
            8, 9, 10, 10, 11, 8, // Top
            12, 13, 14, 14, 15, 12, // Bottom
            16, 17, 18, 18, 19, 16, // Right
            20, 21, 22, 22, 23, 20, // Left
        };

        const Ref<Mesh>& cubeMesh = CreateRef<Mesh>(indices, vertices);
        cubeMesh->SetName("Cube");

        AABB cubeAABB(glm::vec3(-size.x * 0.5f, -size.y * 0.5f, -size.z * 0.5f), glm::vec3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
        cubeMesh->SetAABB(cubeAABB);

        return cubeMesh;
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

        const Ref<Mesh>& sphereMesh = CreateRef<Mesh>(indices, data);
        sphereMesh->SetName("Sphere");

        AABB sphereAABB(glm::vec3(-radius, -radius, -radius), glm::vec3(radius, radius, radius));
        sphereMesh->SetAABB(sphereAABB);

        return sphereMesh;
    }

    Ref<Mesh> PrimitiveMesh::CreateCylinder(float topRadius, float bottomRadius, float height, int radialSegments, int rings, bool capTop, bool capBottom)
    {
        std::vector<Vertex> data;
        std::vector<uint32_t> indices;

        int point = 0;
        float angleStep = glm::two_pi<float>() / radialSegments;
        float heightStep = height / rings;

        // Generate vertices and indices for the side surface
        for (int j = 0; j <= rings; ++j)
        {
            float v = j / float(rings);
            float y = height * 0.5f - j * heightStep;
            float radius = glm::mix(topRadius, bottomRadius, v);

            for (int i = 0; i <= radialSegments; ++i)
            {
                float angle = i * angleStep;
                float x = radius * glm::cos(angle);
                float z = radius * glm::sin(angle);

                Vertex vertex;
                vertex.Position = glm::vec3(x, y, z);
                vertex.Normals = glm::normalize(glm::vec3(x, 0.0f, z));
                vertex.TexCoords = glm::vec2(i / float(radialSegments), v);
                data.emplace_back(vertex);

                if (i > 0 && j > 0)
                {
                    int a = point - 1;
                    int b = point;
                    int c = point - (radialSegments + 1) - 1;
                    int d = point - (radialSegments + 1);

                    indices.push_back(a);
                    indices.push_back(b);
                    indices.push_back(c);

                    indices.push_back(b);
                    indices.push_back(d);
                    indices.push_back(c);
                }
                point++;
            }
        }

        // Generate vertices and indices for the top cap
        if (capTop)
        {
            int topCenterIndex = point;
            Vertex topCenterVertex;
            topCenterVertex.Position = glm::vec3(0.0f, height * 0.5f, 0.0f);
            topCenterVertex.Normals = glm::vec3(0.0f, 1.0f, 0.0f);
            topCenterVertex.TexCoords = glm::vec2(0.5f, 0.5f);
            data.emplace_back(topCenterVertex);
            point++;

            for (int i = 0; i <= radialSegments; ++i)
            {
                float angle = i * angleStep;
                float x = topRadius * glm::cos(angle);
                float z = topRadius * glm::sin(angle);

                Vertex vertex;
                vertex.Position = glm::vec3(x, height * 0.5f, z);
                vertex.Normals = glm::vec3(0.0f, 1.0f, 0.0f);
                vertex.TexCoords = glm::vec2((x / topRadius + 1.0f) * 0.5f, (z / topRadius + 1.0f) * 0.5f);
                data.emplace_back(vertex);

                if (i > 0)
                {
                    indices.push_back(topCenterIndex);
                    indices.push_back(point - 1);
                    indices.push_back(point);
                }
                point++;
            }
        }

        // Generate vertices and indices for the bottom cap
        if (capBottom)
        {
            int bottomCenterIndex = point;
            Vertex bottomCenterVertex;
            bottomCenterVertex.Position = glm::vec3(0.0f, -height * 0.5f, 0.0f);
            bottomCenterVertex.Normals = glm::vec3(0.0f, -1.0f, 0.0f);
            bottomCenterVertex.TexCoords = glm::vec2(0.5f, 0.5f);
            data.emplace_back(bottomCenterVertex);
            point++;

            for (int i = 0; i <= radialSegments; ++i)
            {
                float angle = i * angleStep;
                float x = bottomRadius * glm::cos(angle);
                float z = bottomRadius * glm::sin(angle);

                Vertex vertex;
                vertex.Position = glm::vec3(x, -height * 0.5f, z);
                vertex.Normals = glm::vec3(0.0f, -1.0f, 0.0f);
                vertex.TexCoords = glm::vec2((x / bottomRadius + 1.0f) * 0.5f, (z / bottomRadius + 1.0f) * 0.5f);
                data.emplace_back(vertex);

                if (i > 0)
                {
                    indices.push_back(bottomCenterIndex);
                    indices.push_back(point);
                    indices.push_back(point - 1);
                }
                point++;
            }
        }

        const Ref<Mesh>& cylinderMesh = CreateRef<Mesh>(indices, data);
        cylinderMesh->SetName("Cylinder");

        AABB cylinderAABB(glm::vec3(-topRadius, -height * 0.5f, -topRadius), glm::vec3(topRadius, height * 0.5f, topRadius));
        cylinderMesh->SetAABB(cylinderAABB);
        
        return cylinderMesh;
    }

    Ref<Mesh> PrimitiveMesh::CreateCone(float radius, float height, int radialSegments, int rings, bool cap)
    {
        std::vector<Vertex> data;
        std::vector<uint32_t> indices;

        int point = 0;
        float angleStep = glm::two_pi<float>() / radialSegments;

        // Generate vertices and indices for the side surface
        for (int i = 0; i <= radialSegments; ++i)
        {
            float angle = i * angleStep;
            float x = radius * glm::cos(angle);
            float z = radius * glm::sin(angle);

            // Side vertex
            Vertex vertex;
            vertex.Position = glm::vec3(x, 0.0f, z);
            vertex.Normals = glm::normalize(glm::vec3(x, radius / height, z));
            vertex.TexCoords = glm::vec2(i / float(radialSegments), 1.0f);
            data.emplace_back(vertex);

            // Top vertex (apex)
            vertex.Position = glm::vec3(0.0f, height, 0.0f);
            vertex.Normals = glm::normalize(glm::vec3(x, radius / height, z));
            vertex.TexCoords = glm::vec2(i / float(radialSegments), 0.0f);
            data.emplace_back(vertex);

            if (i > 0)
            {
                int baseIndex = (i - 1) * 2;
                indices.push_back(baseIndex);
                indices.push_back(baseIndex + 1);
                indices.push_back(baseIndex + 2);

                indices.push_back(baseIndex + 1);
                indices.push_back(baseIndex + 3);
                indices.push_back(baseIndex + 2);
            }
            point += 2;
        }

        // Generate vertices and indices for the base
        if (cap)
        {
            int baseCenterIndex = point;
            Vertex baseCenterVertex;
            baseCenterVertex.Position = glm::vec3(0.0f, 0.0f, 0.0f);
            baseCenterVertex.Normals = glm::vec3(0.0f, -1.0f, 0.0f);
            baseCenterVertex.TexCoords = glm::vec2(0.5f, 0.5f);
            data.emplace_back(baseCenterVertex);
            point++;

            for (int i = 0; i <= radialSegments; ++i)
            {
                float angle = i * angleStep;
                float x = radius * glm::cos(angle);
                float z = radius * glm::sin(angle);

                Vertex vertex;
                vertex.Position = glm::vec3(x, 0.0f, z);
                vertex.Normals = glm::vec3(0.0f, -1.0f, 0.0f);
                vertex.TexCoords = glm::vec2((x / radius + 1.0f) * 0.5f, (z / radius + 1.0f) * 0.5f);
                data.emplace_back(vertex);

                if (i > 0)
                {
                    indices.push_back(baseCenterIndex);
                    indices.push_back(point - 1);
                    indices.push_back(point);
                }
                point++;
            }
        }

        const Ref<Mesh>& coneMesh = CreateRef<Mesh>(indices, data);
        coneMesh->SetName("Cone");

        AABB coneAABB(glm::vec3(-radius, 0.0f, -radius), glm::vec3(radius, height, radius));
        coneMesh->SetAABB(coneAABB);

        return coneMesh;
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

        const Ref<Mesh>& torusMesh = CreateRef<Mesh>(indices, data);
        torusMesh->SetName("Torus");

        AABB torusAABB(glm::vec3(-outerRadius, -radius, -outerRadius), glm::vec3(outerRadius, radius, outerRadius));
        torusMesh->SetAABB(torusAABB);

        return torusMesh;
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

        const Ref<Mesh>& capsuleMesh = CreateRef<Mesh>(indices, data);
        capsuleMesh->SetName("Capsule");

        AABB capsuleAABB(glm::vec3(-radius, -0.5f * height, -radius), glm::vec3(radius, 0.5f * height, radius));
        capsuleMesh->SetAABB(capsuleAABB);

        return capsuleMesh;
    }

} // namespace Coffee