#include "Mesh.h"

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/ImportData/ImportData.h"
#include "CoffeeEngine/IO/ImportData/MeshImportData.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/Renderer/Buffer.h"
#include "CoffeeEngine/Renderer/Material.h"
#include "CoffeeEngine/Renderer/VertexArray.h"
#include "CoffeeEngine/Math/BoundingBox.h"
#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <tracy/Tracy.hpp>

namespace Coffee {

    template<class Archive>
    void Vertex::serialize(Archive& archive)
    {
        archive(Position, TexCoords, Normals, Tangent, Bitangent, BoneIDs, BoneWeights);
    }

    // Mesh implementation
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
        : Resource(ResourceType::Mesh)
        , m_Vertices(vertices)
        , m_Indices(indices)
    {
        ZoneScoped;

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

    Mesh::Mesh(const ImportData& importData)
        : Resource(ResourceType::Mesh)
    {
        ZoneScoped;

        const MeshImportData& meshImportData = dynamic_cast<const MeshImportData&>(importData);

        *this = Mesh(meshImportData.vertices, meshImportData.indices);
        m_Name = meshImportData.name;
        m_UUID = meshImportData.uuid;
        m_Material = meshImportData.material;
        m_AABB = meshImportData.aabb;
        m_FilePath = meshImportData.cachedPath;
    }

    const Ref<VertexArray>& Mesh::GetVertexArray() const
    {
        return m_VertexArray;
    }

    const Ref<VertexBuffer>& Mesh::GetVertexBuffer() const
    {
        return m_VertexBuffer;
    }

    const Ref<IndexBuffer>& Mesh::GetIndexBuffer() const
    {
        return m_IndexBuffer;
    }

    void Mesh::SetMaterial(Ref<Material>& material)
    {
        m_Material = material;
    }

    void Mesh::SetAABB(const AABB& aabb)
    {
        m_AABB = aabb;
    }

    const AABB& Mesh::GetAABB() const
    {
        return m_AABB;
    }

    OBB Mesh::GetOBB(const glm::mat4& transform) const
    {
        return {transform, GetAABB()};
    }

    const Ref<Material>& Mesh::GetMaterial() const
    {
        return m_Material;
    }

    const std::vector<Vertex>& Mesh::GetVertices() const
    {
        return m_Vertices;
    }

    const std::vector<uint32_t>& Mesh::GetIndices() const
    {
        return m_Indices;
    }

    template<class Archive>
    void Mesh::save(Archive& archive) const
    {
        UUID materialUUID = m_Material ? m_Material->GetUUID() : UUID::null;
        archive(m_Vertices, m_Indices, m_AABB, materialUUID, cereal::base_class<Resource>(this));
    }

    template<class Archive>
    void Mesh::load(Archive& archive)
    {
        UUID materialUUID;
        archive(m_Vertices, m_Indices, m_AABB, materialUUID, cereal::base_class<Resource>(this));

        if (materialUUID != UUID::null)
            m_Material = ResourceLoader::GetResource<PBRMaterial>(materialUUID);
    }

    template<class Archive>
    void Mesh::load_and_construct(Archive& data, cereal::construct<Mesh>& construct)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        data(vertices, indices);
        construct(vertices, indices);

        UUID materialUUID;
        data(construct->m_AABB, materialUUID, cereal::base_class<Resource>(construct.ptr()));
        
        construct->m_Vertices = vertices;
        construct->m_Indices = indices;
        
        if (materialUUID != UUID::null)
            construct->m_Material = ResourceLoader::GetResource<PBRMaterial>(materialUUID);
    }

    // Explicit template instantiations for Vertex
    template void Vertex::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&);
    template void Vertex::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&);
    template void Vertex::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&);
    template void Vertex::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&);

    // Explicit template instantiations for Mesh
    template void Mesh::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&) const;
    template void Mesh::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&) const;
    template void Mesh::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&);
    template void Mesh::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&);

    template void Mesh::load_and_construct<cereal::JSONInputArchive>(cereal::JSONInputArchive&, cereal::construct<Mesh>&);
    template void Mesh::load_and_construct<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&, cereal::construct<Mesh>&);

} // namespace Coffee

// Cereal registration
CEREAL_REGISTER_TYPE(Coffee::Mesh);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Resource, Coffee::Mesh);