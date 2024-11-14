#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"
#include "CoffeeEngine/Renderer/Shader.h"
#include "CoffeeEngine/Renderer/Texture.h"
#include <glm/fwd.hpp>
#include <string>

namespace Coffee {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the CoffeeEngine.
     * @{
     */

    // Not implemented yet!!
    struct MaterialRenderSettings
    {
        bool wireframe = false;
        bool depthTest = true;
        bool depthWrite = true;
        bool faceCulling = true;
    };

    /**
     * @brief Structure representing the properties of a material.
     */
    struct MaterialProperties
    {
        glm::vec4 color = glm::vec4(1.0f); ///< The color of the material.
        float metallic = 0.0f; ///< The metallic value of the material.
        float roughness = 1.0f; ///< The roughness value of the material.
        float ao = 1.0f; ///< The ambient occlusion value of the material.
        glm::vec3 emissive = glm::vec3(0.0f); ///< The emissive value of the material.
    };

    /**
     * @brief Structure representing the textures used in a material.
     */
    struct MaterialTextures // Temporal
    {
        Ref<Texture> albedo; ///< The albedo texture.
        Ref<Texture> normal; ///< The normal map texture.
        Ref<Texture> metallic; ///< The metallic texture.
        Ref<Texture> roughness; ///< The roughness texture.
        Ref<Texture> ao; ///< The ambient occlusion texture.
        Ref<Texture> emissive; ///< The emissive texture.
    };

    struct MaterialTextureFlags
    {
        bool hasAlbedo = false; ///< Whether the material has an albedo texture.
        bool hasNormal = false; ///< Whether the material has a normal map texture.
        bool hasMetallic = false; ///< Whether the material has a metallic texture.
        bool hasRoughness = false; ///< Whether the material has a roughness texture.
        bool hasAO = false; ///< Whether the material has an ambient occlusion texture.
        bool hasEmissive = false; ///< Whether the material has an emissive texture.
    };

    /**
     * @brief Class representing a material.
     */
    class Material : public Resource
    {
    public:
        /**
         * @brief Default constructor for the Material class.
         */
        Material(const std::string& name);

        /**
         * @brief Constructs a Material with the specified shader.
         * @param shader The shader to be used with the material.
         */
        Material(const std::string& name, Ref<Shader> shader);

        /**
         * @brief Constructs a Material from a file path.
         * @param path The file path to the material definition.
         * @note This constructor is for future use when the material YAML exists.
         */
        Material(std::string& path);

        /**
         * @brief Constructs a Material with the specified textures.
         * @param materialTextures The textures to be used with the material.
         */
        Material(const std::string& name, MaterialTextures& materialTextures);

        /**
         * @brief Default destructor for the Material class.
         */
        ~Material() = default;

        /**
         * @brief Uses the material by binding its shader and textures.
         */
        void Use();

        /**
         * @brief Gets the shader associated with the material.
         * @return A reference to the shader.
         */
        Ref<Shader> GetShader() { return m_Shader; }

        MaterialTextures& GetMaterialTextures() { return m_MaterialTextures; }
        MaterialProperties& GetMaterialProperties() { return m_MaterialProperties; }

        //TODO: Remove the materialTextures parameter and make a function that set the materialTextures and the shader too
        static Ref<Material> Create(const std::string& name = "Material" /* + uuid*/, MaterialTextures* materialTextures = nullptr);

    private:
        MaterialTextures m_MaterialTextures; ///< The textures used in the material.
        MaterialTextureFlags m_MaterialTextureFlags; ///< The flags for the textures used in the material.
        MaterialProperties m_MaterialProperties; ///< The properties of the material.
        MaterialRenderSettings m_MaterialRenderSettings; ///< The render settings of the material.
        Ref<Shader> m_Shader; ///< The shader used with the material.
        static Ref<Texture> s_MissingTexture; ///< The texture to use when a texture is missing.
    };

    /** @} */
}