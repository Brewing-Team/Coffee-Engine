#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Buffer.h"
#include <cstdint>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Tea {

    struct Vertex {

        glm::vec3 Position = glm::vec3(0.0f);
        glm::vec2 TexCoords = glm::vec2(0.0f);
        glm::vec3 Normals = glm::vec3(0.0f);
        glm::vec3 Tangent = glm::vec3(0.0f);
        glm::vec3 Bitangent = glm::vec3(0.0f);

    };

    class Mesh
    {
    public:
        Mesh(const std::vector<uint32_t>& indices, const std::vector<Vertex>& vertices);

        const Ref<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
        const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
    private:
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;

        std::string m_Name;

        std::vector<uint32_t> m_Indices;
        std::vector<Vertex> m_Vertices;
    };

}