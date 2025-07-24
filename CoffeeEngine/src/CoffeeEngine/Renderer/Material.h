#pragma once

#include "CoffeeEngine/Core/Base.h"
#include "CoffeeEngine/IO/Resource.h"
#include <string>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float3.hpp>

// Forward declarations
namespace Coffee {
    class Shader;
    class Texture2D;
    class Project;
    class ImportData;
    class ResourceLoader;
    struct UUID;
}

namespace Coffee {

    /**
     * @defgroup renderer Renderer
     * @brief Renderer components of the CoffeeEngine.
     * @{
     */

    struct MaterialRenderSettings
    {
        // Transparency
        enum TransparencyMode
        {
            Disabled = 0,
            Alpha,
            AlphaCutoff
        } transparencyMode = TransparencyMode::Disabled; ///< The transparency mode.

        float alphaCutoff = 0.5f; ///< The alpha cutoff value for the transparency mode.

        // Blend mode
        enum BlendMode
        {
            Mix = 0,
            Add,
            Subtract,
            Multiply
        } blendMode = BlendMode::Mix; ///< The blend mode for the transparency.

        // Culling
        enum CullMode
        {
            Front = 0,
            Back,
            None
        } cullMode = CullMode::Back; ///< The culling mode for the PBRMaterial.

        // Depth
/*         enum DepthMode
        {
            Read = 0,
            Write,
            None
        } depthMode = DepthMode::Write; ///< The depth mode for the PBRMaterial. */
        bool depthTest = true; ///< Whether to enable depth testing.

        // Wireframe
        bool wireframe = false; ///< Whether to render the PBRMaterial in wireframe mode.
        
    private:
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive& archive);
    };

    class Material : public Resource
    {
    public:
        Material() = default;
        Material(ResourceType type);
        Material(const std::string& name, ResourceType type);
        Material(const std::string& name, Ref<Shader> shader, ResourceType type);

        Ref<Shader> GetShader() const;
        MaterialRenderSettings& GetRenderSettings();
        const MaterialRenderSettings& GetRenderSettings() const;

        virtual void Use() = 0;

    private:
        friend class cereal::access;
        template<class Archive>
        void save(Archive& archive) const;
        template<class Archive>
        void load(Archive& archive);

    protected:
        Ref<Shader> m_Shader; ///< The shader used by the material.
        MaterialRenderSettings m_RenderSettings; ///< The render settings for the material.
    };

    class ShaderMaterial : public Material
    {
    public:
        ShaderMaterial();
        ShaderMaterial(const std::string& name, Ref<Shader> shader);
        ShaderMaterial(ImportData& importData);

        void Use() override;
        void SetShader(Ref<Shader> shader);

        static Ref<ShaderMaterial> Create(const std::string& name = "", Ref<Shader> shader = nullptr);

    private:
        friend class cereal::access;
        template<class Archive>
        void save(Archive& archive) const;
        template<class Archive>
        void load(Archive& archive);
    };

    /**
     * @brief Structure representing the properties of a PBRMaterial.
     */
    struct PBRMaterialProperties
    {
        glm::vec4 color; ///< The color of the PBRMaterial.
        float metallic = 0.0f; ///< The metallic value of the PBRMaterial.
        float roughness = 1.0f; ///< The roughness value of the PBRMaterial.
        float ao = 1.0f; ///< The ambient occlusion value of the PBRMaterial.
        glm::vec3 emissive; ///< The emissive value of the PBRMaterial.

        PBRMaterialProperties();

    private:
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive& archive);
    };

    /**
     * @brief Structure representing the textures used in a PBRMaterial.
     */
    struct PBRMaterialTextures // Temporal
    {
        Ref<Texture2D> albedo; ///< The albedo texture.
        Ref<Texture2D> normal; ///< The normal map texture.
        Ref<Texture2D> metallic; ///< The metallic texture.
        Ref<Texture2D> roughness; ///< The roughness texture.
        Ref<Texture2D> ao; ///< The ambient occlusion texture.
        Ref<Texture2D> emissive; ///< The emissive texture.

    private:
        friend class cereal::access;
        template<class Archive>
        void save(Archive& archive) const;
        template<class Archive>
        void load(Archive& archive);
    };

    struct PBRMaterialTextureFlags
    {
        bool hasAlbedo = false; ///< Whether the PBRMaterial has an albedo texture.
        bool hasNormal = false; ///< Whether the PBRMaterial has a normal map texture.
        bool hasMetallic = false; ///< Whether the PBRMaterial has a metallic texture.
        bool hasRoughness = false; ///< Whether the PBRMaterial has a roughness texture.
        bool hasAO = false; ///< Whether the PBRMaterial has an ambient occlusion texture.
        bool hasEmissive = false; ///< Whether the PBRMaterial has an emissive texture.

    private:
        friend class cereal::access;
        template<class Archive>
        void serialize(Archive& archive);
    };

    /**
     * @brief Class representing a PBRMaterial.
     */
    class PBRMaterial : public Material
    {
    public:
        PBRMaterial();
        
        /**
         * @brief Default destructor for the PBRMaterial class.
         */
        ~PBRMaterial() = default;

        /**
         * @brief Default constructor for the PBRPBRMaterial class.
         */
        PBRMaterial(const std::string& name);

        /**
         * @brief Constructs a PBRMaterial with the specified shader.
         * @param shader The shader to be used with the PBRMaterial.
         */
        PBRMaterial(const std::string& name, Ref<Shader> shader);

        /**
         * @brief Constructs a PBRMaterial with the specified textures.
         * @param textures The textures to be used with the PBRMaterial.
         */
        PBRMaterial(const std::string& name, PBRMaterialTextures& textures);

        PBRMaterial(ImportData& importData);

        void Use() override;

        PBRMaterialTextures& GetTextures();
        PBRMaterialProperties& GetProperties();
        const PBRMaterialTextures& GetTextures() const;
        const PBRMaterialProperties& GetProperties() const;

        //TODO: Remove the PBRMaterialTextures parameter and make a function that set the PBRMaterialTextures and the shader too
        static Ref<PBRMaterial> Create(const std::string& name = "", PBRMaterialTextures* textures = nullptr);

    private:
        friend class cereal::access;
        template<class Archive>
        void save(Archive& archive) const;
        template<class Archive>
        void load(Archive& archive);
        template<class Archive>
        static void load_and_construct(Archive& archive, cereal::construct<PBRMaterial>& construct);

    private:
        PBRMaterialTextures m_Textures; ///< The textures used in the PBRMaterial.
        PBRMaterialTextureFlags m_TextureFlags; ///< The flags for the textures used in the PBRMaterial.
        PBRMaterialProperties m_Properties; ///< The properties of the PBRMaterial.
        static Ref<Texture2D> s_MissingTexture; ///< The texture to use when a texture is missing.
        static Ref<Shader> s_StandardShader; ///< The standard shader to use with the PBRMaterial. (When the Material be a base class of PBRMaterial and ShaderMaterial this should be moved to PBRMaterial)
    };

    /** @} */
}