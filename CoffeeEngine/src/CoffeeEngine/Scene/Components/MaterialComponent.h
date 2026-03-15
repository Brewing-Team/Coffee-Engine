#pragma once

#include "CoffeeEngine/Core/Base.h"
#include <cereal/cereal.hpp>

namespace Coffee
{
    class Material;
    struct EngineContext;
}

namespace Coffee
{
    /**
     * @brief Component representing a material.
     * @ingroup scene
     */
    struct MaterialComponent
    {
        ResourceRef<Material> material; ///< The material reference.

        ResourceID pendingMaterialID = ResourceID::null;
        ResourceType pendingMaterialType = ResourceType::Unknown;

        MaterialComponent() {}
        MaterialComponent(const MaterialComponent&) = default;
        MaterialComponent(ResourceRef<Material> material) : material(material) {}

        void ResolveResources(EngineContext& context);

      private:
        friend class cereal::access;
        /**
         * @brief Serializes the MaterialComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template <class Archive> void save(Archive& archive, std::uint32_t const version) const;
        template <class Archive> void load(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::MaterialComponent, 1);
