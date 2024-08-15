#include "Material.h"
#include <glm/fwd.hpp>
#include <tracy/Tracy.hpp>

namespace Tea {

    Material::Material()
    {
        ZoneScoped;

        textures.albedo = Texture::Load("assets/textures/UVMap-Grid.jpg");

        m_Shader = Tea::Shader::Create("assets/shaders/StandardShader.vert", "assets/shaders/StandardShader.frag");

        m_Shader->Bind();
        textures.albedo->Bind(0);
        m_Shader->setInt("albedo", 0);
        m_Shader->Unbind();
    }

    Material::Material(Ref<Shader> shader) : m_Shader(shader) {}

    Material::Material(MaterialTextures& materialTextures)
    {
        ZoneScoped;

        textures.albedo = materialTextures.albedo;
        textures.normal = materialTextures.normal;
        textures.metallic = materialTextures.metallic;
        textures.roughness = materialTextures.roughness;
        textures.ao = materialTextures.ao;

        m_Shader = Tea::Shader::Create("assets/shaders/StandardShader.vert", "assets/shaders/StandardShader.frag");

        m_Shader->Bind();
        m_Shader->setInt("albedo", 0);
        m_Shader->setInt("normal", 1);
        m_Shader->setInt("metallic", 2);
        m_Shader->setInt("roughness", 3);
        m_Shader->setInt("ao", 4);
        m_Shader->Unbind();
    }

    void Material::Use()
    {
        ZoneScoped;

        m_Shader->Bind();
        if(textures.albedo)textures.albedo->Bind(0);
        if(textures.normal)textures.normal->Bind(1);
        if(textures.metallic)textures.metallic->Bind(2);
        if(textures.roughness)textures.roughness->Bind(3);
        if(textures.ao)textures.ao->Bind(4);
    }
}
