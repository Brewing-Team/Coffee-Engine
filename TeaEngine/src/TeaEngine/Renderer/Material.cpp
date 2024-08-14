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

        m_Shader = Tea::Shader::Create("assets/shaders/StandardShader.vert", "assets/shaders/StandardShader.frag");

        m_Shader->Bind();
        textures.albedo->Bind(0);
        m_Shader->setInt("albedo", 0);
        m_Shader->Unbind();
    }

    void Material::Use()
    {
        ZoneScoped;

        m_Shader->Bind();
        if(textures.albedo)textures.albedo->Bind(0);
    }
}
