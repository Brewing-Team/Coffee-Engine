#pragma once

#include "CoffeeEngine/Core/Base.h"
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

    /**
     * @brief Structure representing the properties of a material.
     */
    struct MaterialProperties
    {
        // Add material properties here
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

    /**
     * @brief Class representing a material.
     */
    class Material
    {
    public:
        /**
         * @brief Default constructor for the Material class.
         */
        Material();

        /**
         * @brief Constructs a Material with the specified shader.
         * @param shader The shader to be used with the material.
         */
        Material(Ref<Shader> shader);

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
        Material(MaterialTextures& materialTextures);

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

        const MaterialTextures& GetMaterialTextures() const { return m_MaterialTextures; }

        //For testing purposes is not definitive

        void SetAlbedoTexture(Ref<Texture> texture) { m_MaterialTextures.albedo = texture; }

    private:
        MaterialTextures m_MaterialTextures; ///< The textures used in the material.
        Ref<Shader> m_Shader; ///< The shader used with the material.
        static Ref<Texture> s_MissingTexture; ///< The texture to use when a texture is missing.
    };

    /** @} */
}