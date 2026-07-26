#pragma once

#include "CoffeeEngine/Core/Base.h"

#include <glm/vec3.hpp>
#include <cereal/cereal.hpp>

namespace Coffee
{
    class Cubemap;

}

namespace Coffee
{
    struct WorldEnvironmentComponent
    {
        // Skybox
        Ref<Cubemap> Skybox; ///< The skybox reference.
        float SkyboxIntensity = 1.0f; ///< The exposure of the skybox.

        // Tonemapping
        float TonemappingExposure = 1.0f; ///< The exposure for tonemapping.

        bool Fog = false;                        ///< Flag to enable fog.
        glm::vec3 FogColor = {0.5f, 0.5f, 0.5f}; ///< The color of the fog.
        float FogDensity = 0.1f;                 ///< The density of the fog.
        float FogHeight = 0.0f; ///< Fog height.
        float FogHeightDensity = 0.1f; ///< Fog height density.///< The end distance of the fog.

        // Mockup
        bool SSAO = false;          ///< Flag to enable screen space ambient occlusion.
        float SSAORadius = 0.5f;    ///< The radius of the SSAO.
        float SSAOStrength = 1.0f;  ///< The strength of the SSAO.
        float SSAOIntensity = 1.0f; ///< The intensity of the SSAO.
        float SSAOScale = 1.0f;     ///< The scale of the SSAO.
        float SSAOBias = 0.1f;      ///< The bias of the SSAO.

        bool Bloom = false;          ///< Flag to enable bloom.
        int BloomMaxMipLevels = 5; ///< The maximum number of mip levels for bloom.
        float BloomIntensity = 1.0f; ///< The intensity of the bloom.
        float BloomRadius = 1.0f;    ///< The radius of the bloom.

        WorldEnvironmentComponent() = default;
        WorldEnvironmentComponent(const WorldEnvironmentComponent&) = default;

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;
        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::WorldEnvironmentComponent, 2);
