#include "Material.h"

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/ImportData/ImportData.h"
#include "CoffeeEngine/IO/ImportData/MaterialImportData.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/IO/ResourceLoader.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include "CoffeeEngine/Project/Project.h"
#include "CoffeeEngine/Embedded/StandardShader.inl"
#include "CoffeeEngine/IO/Serialization/GLMSerialization.h"
#include "CoffeeEngine/IO/Serialization/FilesystemPathSerialization.h"
#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <filesystem>
#include <tracy/Tracy.hpp>

namespace Coffee {

    // MaterialRenderSettings implementation
    template<class Archive>
    void MaterialRenderSettings::serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("TransparencyMode", transparencyMode),
            cereal::make_nvp("AlphaCutoff", alphaCutoff),
            cereal::make_nvp("BlendMode", blendMode),
            cereal::make_nvp("CullMode", cullMode),
            cereal::make_nvp("DepthTest", depthTest),
            cereal::make_nvp("Wireframe", wireframe)
        );
    }

    // Material implementation

    Material::Material(ResourceType type) : Resource(type) {}

    Material::Material(const std::string& name, ResourceType type) : Resource(type) 
    { 
        m_Name = name; 
    }

    Material::Material(const std::string& name, Ref<Shader> shader, ResourceType type) 
        : Resource(type), m_Shader(shader) 
    { 
        m_Name = name; 
    }

    Ref<Shader> Material::GetShader() const 
    { 
        return m_Shader; 
    }

    MaterialRenderSettings& Material::GetRenderSettings() 
    { 
        return m_RenderSettings; 
    }

    const MaterialRenderSettings& Material::GetRenderSettings() const 
    { 
        return m_RenderSettings; 
    }

    template<class Archive>
    void Material::save(Archive& archive) const
    {
        std::filesystem::path shaderPath = m_Shader ? m_Shader->GetPath() : "";
        
        if (Project::GetActive())
            shaderPath = std::filesystem::relative(m_Shader->GetPath(), Project::GetActive()->GetProjectDirectory());
        else
            shaderPath = m_Shader->GetPath();

        archive(cereal::base_class<Resource>(this), 
                cereal::make_nvp("Shader Path", shaderPath.generic_string()), 
                cereal::make_nvp("Render Settings", m_RenderSettings));
    }

    template<class Archive>
    void Material::load(Archive& archive)
    {
        std::string shaderPath;
        archive(cereal::base_class<Resource>(this), 
                cereal::make_nvp("Shader Path", shaderPath), 
                cereal::make_nvp("Render Settings", m_RenderSettings));

        if (Project::GetActive())
            shaderPath = (Project::GetActive()->GetProjectDirectory() / shaderPath).string();

        if (!shaderPath.empty() && std::filesystem::is_regular_file(shaderPath)) 
            m_Shader = Shader::Create(shaderPath);
    }

    // ShaderMaterial implementation
    ShaderMaterial::ShaderMaterial() : Material(ResourceType::ShaderMaterial) {}

    ShaderMaterial::ShaderMaterial(const std::string& name, Ref<Shader> shader) 
        : Material(name, shader, ResourceType::ShaderMaterial) {}

    ShaderMaterial::ShaderMaterial(ImportData& importData)
        : Material(ResourceType::ShaderMaterial)
    {
        ZoneScoped;

        m_UUID = importData.uuid;
        m_Name = importData.originalPath.stem().string();
        m_Shader = Shader::Create(importData.originalPath);
        m_FilePath = importData.cachedPath;
    }

    void ShaderMaterial::Use()
    {
        ZoneScoped;

        if (m_Shader)
        {
            m_Shader->Bind();
        }
    }

    void ShaderMaterial::SetShader(Ref<Shader> shader) 
    { 
        m_Shader = shader; 
    }

    Ref<ShaderMaterial> ShaderMaterial::Create(const std::string& name, Ref<Shader> shader)
    {
        return CreateRef<ShaderMaterial>(name, shader);
    }

    template<class Archive>
    void ShaderMaterial::save(Archive& archive) const
    {
        archive(cereal::base_class<Material>(this));
    }

    template<class Archive>
    void ShaderMaterial::load(Archive& archive)
    {
        archive(cereal::base_class<Material>(this));
    }

    // PBRMaterialProperties implementation
    PBRMaterialProperties::PBRMaterialProperties()
        : color(1.0f), emissive(0.0f)
    {
    }

    template<class Archive>
    void PBRMaterialProperties::serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Color", color),
            cereal::make_nvp("Metallic", metallic),
            cereal::make_nvp("Roughness", roughness),
            cereal::make_nvp("AO", ao),
            cereal::make_nvp("Emissive", emissive)
        );
    }

    // PBRMaterialTextures implementation
    template<class Archive>
    void PBRMaterialTextures::save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("AlbedoUUID", albedo ? albedo->GetUUID() : UUID::null),
            cereal::make_nvp("NormalUUID", normal ? normal->GetUUID() : UUID::null),
            cereal::make_nvp("MetallicUUID", metallic ? metallic->GetUUID() : UUID::null),
            cereal::make_nvp("RoughnessUUID", roughness ? roughness->GetUUID() : UUID::null),
            cereal::make_nvp("AOUUID", ao ? ao->GetUUID() : UUID::null),
            cereal::make_nvp("EmissiveUUID", emissive ? emissive->GetUUID() : UUID::null)
        );
    }

    template<class Archive>
    void PBRMaterialTextures::load(Archive& archive)
    {
        UUID albedoUUID, normalUUID, metallicUUID, roughnessUUID, aoUUID, emissiveUUID;
        archive(
            cereal::make_nvp("AlbedoUUID", albedoUUID),
            cereal::make_nvp("NormalUUID", normalUUID),
            cereal::make_nvp("MetallicUUID", metallicUUID),
            cereal::make_nvp("RoughnessUUID", roughnessUUID),
            cereal::make_nvp("AOUUID", aoUUID),
            cereal::make_nvp("EmissiveUUID", emissiveUUID)
        );

        albedo = ResourceLoader::GetResource<Texture2D>(albedoUUID);
        normal = ResourceLoader::GetResource<Texture2D>(normalUUID);
        metallic = ResourceLoader::GetResource<Texture2D>(metallicUUID);
        roughness = ResourceLoader::GetResource<Texture2D>(roughnessUUID);
        ao = ResourceLoader::GetResource<Texture2D>(aoUUID);
        emissive = ResourceLoader::GetResource<Texture2D>(emissiveUUID);
    }

    // PBRMaterialTextureFlags implementation
    template<class Archive>
    void PBRMaterialTextureFlags::serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("HasAlbedo", hasAlbedo),
            cereal::make_nvp("HasNormal", hasNormal),
            cereal::make_nvp("HasMetallic", hasMetallic),
            cereal::make_nvp("HasRoughness", hasRoughness),
            cereal::make_nvp("HasAO", hasAO),
            cereal::make_nvp("HasEmissive", hasEmissive)
        );
    }

    // PBRMaterial static members
    Ref<Texture2D> PBRMaterial::s_MissingTexture = nullptr;
    Ref<Shader> PBRMaterial::s_StandardShader = nullptr;

    // PBRMaterial implementation
    PBRMaterial::PBRMaterial() : Material(ResourceType::PBRMaterial)
    {
        s_StandardShader = s_StandardShader ? s_StandardShader : CreateRef<Shader>("StandardShader", std::string(standardShaderSource));
        m_Shader = s_StandardShader;
    }

    PBRMaterial::PBRMaterial(const std::string& name)
        : Material(name, ResourceType::PBRMaterial)
    {
        ZoneScoped;

        s_MissingTexture = Texture2D::Load("assets/textures/UVMap-Grid.jpg");
        s_StandardShader = s_StandardShader ? s_StandardShader : CreateRef<Shader>("StandardShader", std::string(standardShaderSource));

        m_Textures.albedo = s_MissingTexture;
        m_TextureFlags.hasAlbedo = true;

        m_Shader = s_StandardShader;

        m_Shader->Bind();
        m_Textures.albedo->Bind(0);
        m_Shader->setInt("material.albedoMap", 0);
        m_Shader->Unbind();
    }

    PBRMaterial::PBRMaterial(const std::string& name, Ref<Shader> shader) 
        : Material(name, shader, ResourceType::PBRMaterial) {}

    PBRMaterial::PBRMaterial(const std::string& name, PBRMaterialTextures& materialTextures)
        : Material(name, ResourceType::PBRMaterial)
    {
        ZoneScoped;

        s_StandardShader = s_StandardShader ? s_StandardShader : CreateRef<Shader>("StandardShader", std::string(standardShaderSource));

        m_Textures.albedo = materialTextures.albedo;
        m_Textures.normal = materialTextures.normal;
        m_Textures.metallic = materialTextures.metallic;
        m_Textures.roughness = materialTextures.roughness;
        m_Textures.ao = materialTextures.ao;
        m_Textures.emissive = materialTextures.emissive;

        m_TextureFlags.hasAlbedo = (m_Textures.albedo != nullptr);
        m_TextureFlags.hasNormal = (m_Textures.normal != nullptr);
        m_TextureFlags.hasMetallic = (m_Textures.metallic != nullptr);
        m_TextureFlags.hasRoughness = (m_Textures.roughness != nullptr);
        m_TextureFlags.hasAO = (m_Textures.ao != nullptr);
        m_TextureFlags.hasEmissive = (m_Textures.emissive != nullptr);

        if(m_TextureFlags.hasMetallic) m_Properties.metallic = 1.0f;
        if(m_TextureFlags.hasEmissive) m_Properties.emissive = glm::vec3(1.0f);

        m_Shader = s_StandardShader;

        m_Shader->Bind();
        m_Shader->setInt("material.albedoMap", 0);
        m_Shader->setInt("material.normalMap", 1);
        m_Shader->setInt("material.metallicMap", 2);
        m_Shader->setInt("material.roughnessMap", 3);
        m_Shader->setInt("material.aoMap", 4);
        m_Shader->setInt("material.emissiveMap", 5);
        m_Shader->Unbind();
    }

    PBRMaterial::PBRMaterial(ImportData& importData)
        : Material(ResourceType::PBRMaterial)
    {
        PBRMaterialImportData& materialImportData = dynamic_cast<PBRMaterialImportData&>(importData);

        if(materialImportData.materialTextures)
        {
            *this = PBRMaterial(m_Name, *materialImportData.materialTextures);
        }
        else
        {
            *this = PBRMaterial(m_Name);
        }

        m_Name = materialImportData.name;
        m_UUID = materialImportData.uuid;
    }

    void PBRMaterial::Use()
    {
        ZoneScoped;

        // Update Texture Flags
        m_TextureFlags.hasAlbedo = (m_Textures.albedo != nullptr);
        m_TextureFlags.hasNormal = (m_Textures.normal != nullptr);
        m_TextureFlags.hasMetallic = (m_Textures.metallic != nullptr);
        m_TextureFlags.hasRoughness = (m_Textures.roughness != nullptr);
        m_TextureFlags.hasAO = (m_Textures.ao != nullptr);
        m_TextureFlags.hasEmissive = (m_Textures.emissive != nullptr);

        m_Shader->Bind();

        // Bind Textures
        if(m_TextureFlags.hasAlbedo) m_Textures.albedo->Bind(0);
        if(m_TextureFlags.hasNormal) m_Textures.normal->Bind(1);
        if(m_TextureFlags.hasMetallic) m_Textures.metallic->Bind(2);
        if(m_TextureFlags.hasRoughness) m_Textures.roughness->Bind(3);
        if(m_TextureFlags.hasAO) m_Textures.ao->Bind(4);
        if(m_TextureFlags.hasEmissive) m_Textures.emissive->Bind(5);

        // Set Material Properties
        m_Shader->setVec4("material.color", m_Properties.color);
        m_Shader->setFloat("material.metallic", m_Properties.metallic);
        m_Shader->setFloat("material.roughness", m_Properties.roughness);
        m_Shader->setFloat("material.ao", m_Properties.ao);
        m_Shader->setVec3("material.emissive", m_Properties.emissive);

        // Set Material Texture Flags
        m_Shader->setInt("material.hasAlbedo", m_TextureFlags.hasAlbedo);
        m_Shader->setInt("material.hasNormal", m_TextureFlags.hasNormal);
        m_Shader->setInt("material.hasMetallic", m_TextureFlags.hasMetallic);
        m_Shader->setInt("material.hasRoughness", m_TextureFlags.hasRoughness);
        m_Shader->setInt("material.hasAO", m_TextureFlags.hasAO);
        m_Shader->setInt("material.hasEmissive", m_TextureFlags.hasEmissive);

        m_Shader->setInt("material.transparencyMode", m_RenderSettings.transparencyMode);
        m_Shader->setFloat("material.alphaCutoff", m_RenderSettings.alphaCutoff);
    }

    PBRMaterialTextures& PBRMaterial::GetTextures() 
    { 
        return m_Textures; 
    }

    PBRMaterialProperties& PBRMaterial::GetProperties() 
    { 
        return m_Properties; 
    }

    const PBRMaterialTextures& PBRMaterial::GetTextures() const 
    { 
        return m_Textures; 
    }

    const PBRMaterialProperties& PBRMaterial::GetProperties() const 
    { 
        return m_Properties; 
    }

    Ref<PBRMaterial> PBRMaterial::Create(const std::string& name, PBRMaterialTextures* materialTextures)
    {
        PBRMaterialImportData importData;
        importData.name = name;
        importData.materialTextures = materialTextures;
        importData.uuid = UUID();
        importData.cachedPath = CacheManager::GetCachedFilePath(importData.uuid, ResourceType::PBRMaterial);

        return ResourceLoader::LoadEmbedded<PBRMaterial>(importData);
    }

    template<class Archive>
    void PBRMaterial::save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("Textures", m_Textures),
            cereal::make_nvp("TextureFlags", m_TextureFlags),
            cereal::make_nvp("Properties", m_Properties),
            cereal::base_class<Material>(this)
        );
    }

    template<class Archive>
    void PBRMaterial::load(Archive& archive)
    {
        archive(
            cereal::make_nvp("Textures", m_Textures),
            cereal::make_nvp("TextureFlags", m_TextureFlags),
            cereal::make_nvp("Properties", m_Properties),
            cereal::base_class<Material>(this)
        );
    }

    template<class Archive>
    void PBRMaterial::load_and_construct(Archive& archive, cereal::construct<PBRMaterial>& construct)
    {
        PBRMaterialTextures textures;
        PBRMaterialTextureFlags textureFlags;
        PBRMaterialProperties properties;
        PBRMaterial tmpMaterial;

        archive(textures, textureFlags, properties, cereal::base_class<Material>(&tmpMaterial));

        construct(tmpMaterial.GetName(), textures);
        construct->m_TextureFlags = textureFlags;
        construct->m_Properties = properties;
        construct->m_Name = tmpMaterial.GetName();
        construct->m_FilePath = tmpMaterial.GetPath();
        construct->m_Type = tmpMaterial.GetType();
        construct->m_UUID = tmpMaterial.GetUUID();
        construct->m_RenderSettings = tmpMaterial.GetRenderSettings();
    }

    // Explicit template instantiations for common cereal archives
    template void MaterialRenderSettings::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&);
    template void MaterialRenderSettings::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&);
    template void MaterialRenderSettings::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&);
    template void MaterialRenderSettings::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&);

    template void Material::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&) const;
    template void Material::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&) const;
    template void Material::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&);
    template void Material::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&);

    template void ShaderMaterial::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&) const;
    template void ShaderMaterial::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&) const;
    template void ShaderMaterial::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&);
    template void ShaderMaterial::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&);

    template void PBRMaterialProperties::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&);
    template void PBRMaterialProperties::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&);
    template void PBRMaterialProperties::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&);
    template void PBRMaterialProperties::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&);

    template void PBRMaterialTextures::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&) const;
    template void PBRMaterialTextures::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&) const;
    template void PBRMaterialTextures::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&);
    template void PBRMaterialTextures::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&);

    template void PBRMaterialTextureFlags::serialize<cereal::JSONInputArchive>(cereal::JSONInputArchive&);
    template void PBRMaterialTextureFlags::serialize<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&);
    template void PBRMaterialTextureFlags::serialize<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&);
    template void PBRMaterialTextureFlags::serialize<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&);

    template void PBRMaterial::save<cereal::JSONOutputArchive>(cereal::JSONOutputArchive&) const;
    template void PBRMaterial::save<cereal::BinaryOutputArchive>(cereal::BinaryOutputArchive&) const;
    template void PBRMaterial::load<cereal::JSONInputArchive>(cereal::JSONInputArchive&);
    template void PBRMaterial::load<cereal::BinaryInputArchive>(cereal::BinaryInputArchive&);

} // namespace Coffee

// Cereal registration macros
CEREAL_REGISTER_TYPE(Coffee::Material);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Resource, Coffee::Material);
CEREAL_REGISTER_TYPE(Coffee::PBRMaterial);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Material, Coffee::PBRMaterial);
CEREAL_REGISTER_TYPE(Coffee::ShaderMaterial);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::Material, Coffee::ShaderMaterial);