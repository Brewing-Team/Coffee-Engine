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

    Ref<Mesh> PrimitiveMesh::CreateCube(const glm::vec3& size, int subdivideW, int subdivideH, int subdivideD)
    {
        std::vector<Vertex> data;
        std::vector<uint32_t> indices;

        int i, j, prevrow, thisrow, point;
        float x, y, z;

        glm::vec3 startPos = size * -0.5f;

        point = 0;

        // Front and Back faces
        y = startPos.y;
        thisrow = point;
        prevrow = 0;
        for (j = 0; j <= subdivideH + 1; j++)
        {
            float v = j / float(subdivideH + 1);

            x = startPos.x;
            for (i = 0; i <= subdivideW + 1; i++)
            {
                float u = i / float(subdivideW + 1);

                // Front
                Vertex vertex;
                vertex.Position = glm::vec3(x, y, startPos.z);
                vertex.Normals = glm::vec3(0.0f, 0.0f, 1.0f);
                vertex.TexCoords = glm::vec2(u, v);
                data.emplace_back(vertex);
                point++;

                // Back
                vertex.Position = glm::vec3(x, y, -startPos.z);
                vertex.Normals = glm::vec3(0.0f, 0.0f, -1.0f);
                vertex.TexCoords = glm::vec2(1.0f - u, v);
                data.emplace_back(vertex);
                point++;

                if (i > 0 && j > 0) {
                    int i2 = i * 2;

                    // front
                    indices.push_back(prevrow + i2 - 2);
                    indices.push_back(prevrow + i2);
                    indices.push_back(thisrow + i2 - 2);
                    indices.push_back(prevrow + i2);
                    indices.push_back(thisrow + i2);
                    indices.push_back(thisrow + i2 - 2);

                    // back
                    indices.push_back(prevrow + i2 - 1);
                    indices.push_back(prevrow + i2 + 1);
                    indices.push_back(thisrow + i2 - 1);
                    indices.push_back(prevrow + i2 + 1);
                    indices.push_back(thisrow + i2 + 1);
                    indices.push_back(thisrow + i2 - 1);
                }

                x += size.x / (subdivideW + 1.0f);
            }

            y += size.y / (subdivideH + 1.0f);
            prevrow = thisrow;
            thisrow = point;
        }

        // Left and Right faces
        y = startPos.y;
        thisrow = point;
        prevrow = 0;
        for (j = 0; j <= subdivideH + 1; j++)
        {
            float v = j / float(subdivideH + 1);

            z = startPos.z;
            for (i = 0; i <= subdivideD + 1; i++)
            {
                float u = i / float(subdivideD + 1);

                // Right
                Vertex vertex;
                vertex.Position = glm::vec3(startPos.x, y, z);
                vertex.Normals = glm::vec3(-1.0f, 0.0f, 0.0f);
                vertex.TexCoords = glm::vec2(u, v);
                data.emplace_back(vertex);
                point++;

                // Left
                vertex.Position = glm::vec3(-startPos.x, y, z);
                vertex.Normals = glm::vec3(1.0f, 0.0f, 0.0f);
                vertex.TexCoords = glm::vec2(1.0f - u, v);
                data.emplace_back(vertex);
                point++;

                if (i > 0 && j > 0) {
                    int i2 = i * 2;

                    // right
                    indices.push_back(prevrow + i2 - 2);
                    indices.push_back(prevrow + i2);
                    indices.push_back(thisrow + i2 - 2);
                    indices.push_back(prevrow + i2);
                    indices.push_back(thisrow + i2);
                    indices.push_back(thisrow + i2 - 2);

                    // left
                    indices.push_back(prevrow + i2 - 1);
                    indices.push_back(prevrow + i2 + 1);
                    indices.push_back(thisrow + i2 - 1);
                    indices.push_back(prevrow + i2 + 1);
                    indices.push_back(thisrow + i2 + 1);
                    indices.push_back(thisrow + i2 - 1);
                }

                z += size.z / (subdivideD + 1.0f);
            }

            y += size.y / (subdivideH + 1.0f);
            prevrow = thisrow;
            thisrow = point;
        }

        // Top and Bottom faces
        z = startPos.z;
        thisrow = point;
        prevrow = 0;
        for (j = 0; j <= subdivideD + 1; j++)
        {
            float v = j / float(subdivideD + 1);

            x = startPos.x;
            for (i = 0; i <= subdivideW + 1; i++)
            {
                float u = i / float(subdivideW + 1);

                // Top
                Vertex vertex;
                vertex.Position = glm::vec3(x, startPos.y, z);
                vertex.Normals = glm::vec3(0.0f, 1.0f, 0.0f);
                vertex.TexCoords = glm::vec2(u, v);
                data.emplace_back(vertex);
                point++;

                // Bottom
                vertex.Position = glm::vec3(x, -startPos.y, z);
                vertex.Normals = glm::vec3(0.0f, -1.0f, 0.0f);
                vertex.TexCoords = glm::vec2(u, 1.0f - v);
                data.emplace_back(vertex);
                point++;

                if (i > 0 && j > 0) {
                    int i2 = i * 2;

                    // top
                    indices.push_back(prevrow + i2 - 2);
                    indices.push_back(prevrow + i2);
                    indices.push_back(thisrow + i2 - 2);
                    indices.push_back(prevrow + i2);
                    indices.push_back(thisrow + i2);
                    indices.push_back(thisrow + i2 - 2);

                    // bottom
                    indices.push_back(prevrow + i2 - 1);
                    indices.push_back(prevrow + i2 + 1);
                    indices.push_back(thisrow + i2 - 1);
                    indices.push_back(prevrow + i2 + 1);
                    indices.push_back(thisrow + i2 + 1);
                    indices.push_back(thisrow + i2 - 1);
                }

                x += size.x / (subdivideW + 1.0f);
            }

            z += size.z / (subdivideD + 1.0f);
            prevrow = thisrow;
            thisrow = point;
        }

        return CreateRef<Mesh>(indices, data);
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

    /*Ref<Mesh> PrimitiveMesh::CreateCylinder(float topRadius, float bottomRadius, float height, int radialSegments, int
    rings, bool capTop, bool capBottom)
    {
        int i, j, prevrow, thisrow, point;
        float x, y, z, u, v, radius, radiusH;

        std::vector<Vertex> data;
        std::vector<uint32_t> indices;

        thisrow = 0;
        prevrow = 0;
        point = 0;

        const float sideNormalY = (bottomRadius - topRadius) / height;

        // Only used if we calculate UV2
        float topCircumference = topRadius * glm::pi<float>() * 2.0f;
        float bottomCircumference = bottomRadius * glm::pi<float>() * 2.0f;
        float verticalLength = height + std::max(2.0f * topRadius, 2.0f * bottomRadius) + (2.0f * p_uv2_padding);
        float heightV = height / verticalLength;
        float paddingV = p_uv2_padding / verticalLength;

        float horizontalLength = std::max({2.0f * (topRadius + bottomRadius + p_uv2_padding), topCircumference +
    p_uv2_padding, bottomCircumference + p_uv2_padding}); float centerH = 0.5f * (horizontalLength - p_uv2_padding) /
    horizontalLength; float topH = topCircumference / horizontalLength; float bottomH = bottomCircumference /
    horizontalLength; float paddingH = p_uv2_padding / horizontalLength;

        for (j = 0; j <= (rings + 1); j++)
        {
            v = j;
            v /= rings + 1;

            radius = topRadius + (bottomRadius - topRadius) * v;
            radiusH = topH + ((bottomH - topH) * v);

            y = height * v;
            y = (height * 0.5f) - y;

            for (i = 0; i <= radialSegments; i++)
            {
                u = i;
                u /= radialSegments;

                x = sin(u * glm::tau<float>());
                z = cos(u * glm::tau<float>());

                Vertex vertex;
                vertex.Position = glm::vec3(x * radius, y, z * radius);
                vertex.Normals = glm::normalize(glm::vec3(x, sideNormalY, z));
                vertex.Tangent = glm::vec4(z, 0.0f, -x, 1.0f);
                vertex.TexCoords = glm::vec2(u, v * 0.5f);

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
    }*/

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