#include "CoffeeEngine/Renderer/Mesh.h"
#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/ImportData/MeshImportData.h"
#include "CoffeeEngine/Renderer/VertexArray.h"
#include <tracy/Tracy.hpp>

namespace Coffee {

    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
        : Resource(ResourceType::Mesh)
    {
        ZoneScoped;

        m_Vertices = vertices;
        m_Indices = indices;

        m_VertexBuffer = VertexBuffer::Create((float*)m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
        m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), m_Indices.size());

        BufferLayout layout = {
            {ShaderDataType::Vec3, "a_Position"},
            {ShaderDataType::Vec2, "a_TexCoords"},
            {ShaderDataType::Vec3, "a_Normals"},
            {ShaderDataType::Vec3, "a_Tangent"},
            {ShaderDataType::Vec3, "a_Bitangent"},
            {ShaderDataType::IVec4, "a_BoneIDs"},
            {ShaderDataType::Vec4, "a_BoneWeights"}
        };

        m_VertexBuffer->SetLayout(layout);

        m_VertexArray = VertexArray::Create();
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
    }

    Mesh::Mesh(const MeshImportData& importData)
    {
        Mesh(importData.vertices, importData.indices);
        m_Name = importData.name;
        m_UUID = importData.uuid;
        m_Material = importData.material;
        m_AABB = importData.aabb;
        m_FilePath = importData.cachedPath;
    }

}
