#pragma once

#include "TeaEngine/Core/Base.h"
#include "TeaEngine/Renderer/Shader.h"
#include "TeaEngine/Renderer/Texture.h"
#include <glm/fwd.hpp>
#include <string>
namespace Tea {

    struct MaterialTextures //Temporal
    {
        Ref<Texture> albedo;
    };

    class Material
    {
    public:
        Material();
        Material(Ref<Shader> shader);
        Material(std::string& path); //For the future when the material yaml exists
        Material(MaterialTextures& materialTextures);
        ~Material() = default;

        void Use();

        void SetModelMatrix(const glm::mat4& modelMatrix);

        //Temporal
        void SetViewMatrix(const glm::mat4& viewMatrix);
        void SetProjectionMatrix(const glm::mat4& projectionMatrix);

        Ref<Shader> GetShader() { return m_Shader; }
    private:
        MaterialTextures textures;

        Ref<Shader> m_Shader;
    };

}