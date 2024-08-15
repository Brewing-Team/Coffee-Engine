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
        Ref<Texture> normal;
        Ref<Texture> metallic;
        Ref<Texture> roughness;
        Ref<Texture> ao;
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

        Ref<Shader> GetShader() { return m_Shader; }
    private:
        MaterialTextures textures;

        Ref<Shader> m_Shader;
    };

}