#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Buffer.h"
#include "TeaEngine/Renderer/Material.h"
#include "TeaEngine/Renderer/VertexArray.h"
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

        const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
        const Ref<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
        const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName() const { return m_Name; }

        void SetMaterial(Ref<Material>& material) { m_Material = material; };
        const Ref<Material>& GetMaterial() const { return m_Material; }
    private:
        Ref<VertexArray> m_VertexArray;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;

        std::string m_Name;

        Ref<Material> m_Material;

        std::vector<uint32_t> m_Indices;
        std::vector<Vertex> m_Vertices;
    };

}