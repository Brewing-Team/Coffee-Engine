#include "Material.h"
#include <glm/fwd.hpp>
#include <tracy/Tracy.hpp>

namespace Tea {

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
    }

    void Material::SetModelMatrix(const glm::mat4& modelMatrix)
    {
        m_Shader->Bind();
        m_Shader->setMat4("model", modelMatrix);
        m_Shader->Unbind();
    }

    //Temporal
    void Material::SetViewMatrix(const glm::mat4& viewMatrix)
    {
        m_Shader->Bind();
        m_Shader->setMat4("view", viewMatrix);
        m_Shader->Unbind();
    }

    //Temporal
    void Material::SetProjectionMatrix(const glm::mat4& projectionMatrix)
    {
        m_Shader->Bind();
        m_Shader->setMat4("projection", projectionMatrix);
        m_Shader->Unbind();
    }

}
