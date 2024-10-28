#include "Material.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include <glm/fwd.hpp>
#include <tracy/Tracy.hpp>

namespace Coffee {

    Ref<Texture> Material::s_MissingTexture;

    Material::Material()
    {
        ZoneScoped;

        s_MissingTexture = Texture::Load("assets/textures/UVMap-Grid.jpg");

        m_MaterialTextures.albedo = s_MissingTexture;
        m_MaterialTextureFlags.hasAlbedo = true;

        m_Shader = Coffee::Shader::Create("assets/shaders/StandardShader.vert", "assets/shaders/StandardShader.frag");

        m_Shader->Bind();
        m_MaterialTextures.albedo->Bind(0);
        m_Shader->setInt("albedoMap", 0);
        m_Shader->Unbind();
    }

    Material::Material(Ref<Shader> shader) : m_Shader(shader) {}

    Material::Material(MaterialTextures& materialTextures)
    {
        ZoneScoped;

        m_MaterialTextures.albedo = materialTextures.albedo;
        m_MaterialTextures.normal = materialTextures.normal;
        m_MaterialTextures.metallic = materialTextures.metallic;
        m_MaterialTextures.roughness = materialTextures.roughness;
        m_MaterialTextures.ao = materialTextures.ao;
        m_MaterialTextures.emissive = materialTextures.emissive;

        m_MaterialTextureFlags.hasAlbedo = (m_MaterialTextures.albedo != nullptr);
        m_MaterialTextureFlags.hasNormal = (m_MaterialTextures.normal != nullptr);
        m_MaterialTextureFlags.hasMetallic = (m_MaterialTextures.metallic != nullptr);
        m_MaterialTextureFlags.hasRoughness = (m_MaterialTextures.roughness != nullptr);
        m_MaterialTextureFlags.hasAO = (m_MaterialTextures.ao != nullptr);
        m_MaterialTextureFlags.hasEmissive = (m_MaterialTextures.emissive != nullptr);

        m_Shader = Coffee::Shader::Create("assets/shaders/StandardShader.vert", "assets/shaders/StandardShader.frag");

        m_Shader->Bind();
        m_Shader->setInt("albedoMap", 0);
        m_Shader->setInt("normalMap", 1);
        m_Shader->setInt("metallicMap", 2);
        m_Shader->setInt("roughnessMap", 3);
        m_Shader->setInt("aoMap", 4);
        m_Shader->setInt("emissiveMap", 5);
        m_Shader->Unbind();
    }

    void Material::Use()
    {
        ZoneScoped;

        m_MaterialTextureFlags.hasAlbedo = (m_MaterialTextures.albedo != nullptr);
        m_MaterialTextureFlags.hasNormal = (m_MaterialTextures.normal != nullptr);
        m_MaterialTextureFlags.hasMetallic = (m_MaterialTextures.metallic != nullptr);
        m_MaterialTextureFlags.hasRoughness = (m_MaterialTextures.roughness != nullptr);
        m_MaterialTextureFlags.hasAO = (m_MaterialTextures.ao != nullptr);
        m_MaterialTextureFlags.hasEmissive = (m_MaterialTextures.emissive != nullptr);

        m_Shader->Bind();
        if(m_MaterialTextureFlags.hasAlbedo)m_MaterialTextures.albedo->Bind(0);
        if(m_MaterialTextureFlags.hasNormal)m_MaterialTextures.normal->Bind(1);
        if(m_MaterialTextureFlags.hasMetallic)m_MaterialTextures.metallic->Bind(2);
        if(m_MaterialTextureFlags.hasRoughness)m_MaterialTextures.roughness->Bind(3);
        if(m_MaterialTextureFlags.hasAO)m_MaterialTextures.ao->Bind(4);
        if(m_MaterialTextureFlags.hasEmissive)m_MaterialTextures.emissive->Bind(5);
    }
}
