#pragma once

#include <cereal/cereal.hpp>
#include <glm/vec3.hpp>

namespace Coffee
{
    /**
     * @brief Component representing a light.
     * @ingroup scene
     */
    struct LightComponent
    {
        /**
         * @brief Enum representing the type of light.
         */
        enum Type
        {
            DirectionalLight = 0, ///< Directional light.
            PointLight = 1,       ///< Point light.
            SpotLight = 2         ///< Spot light.
        };

        // Align to 16 bytes(glm::vec4) instead of 12 bytes(glm::vec3) to match the std140 layout in the shader (a vec3
        // is 16 bytes in std140)
        alignas(16) glm::vec3 Color = {1.0f, 1.0f, 1.0f};      ///< The color of the light.
        alignas(16) glm::vec3 Direction = {0.0f, -1.0f, 0.0f}; ///< The direction of the light.
        alignas(16) glm::vec3 Position = {0.0f, 0.0f, 0.0f};   ///< The position of the light.

        float Range = 5.0f;       ///< The range of the light.
        float Attenuation = 1.0f; ///< The attenuation of the light.
        float Intensity = 1.0f;   ///< The intensity of the light.

        float Angle = 45.0f; ///< The angle of the light.
        float ConeAttenuation = 48.0f; ///< The cone attenuation of the light.

        int type = static_cast<int>(Type::DirectionalLight); ///< The type of the light.

        // Shadows
        bool Shadow = false;
        float ShadowBias = 0.005f;
        float ShadowMaxDistance = 100.0f;

        LightComponent() = default;
        LightComponent(const LightComponent&) = default;

        /**
         * @brief Serializes the LightComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template <class Archive> void serialize(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::LightComponent, 2);
