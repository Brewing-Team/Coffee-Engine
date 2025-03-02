#pragma once

#include "CoffeeEngine/IO/ImportData/ImportData.h"
#include "CoffeeEngine/Renderer/Material.h"
#include <cereal/cereal.hpp>

namespace Coffee
{

    struct MaterialImportData : public ImportData
    {

        std::string name;
        MaterialTextures materialTextures;

        MaterialImportData() : ImportData(ResourceType::Material) {}

        template <typename Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::base_class<ImportData>(this));
        }
    };

} // namespace Coffee
CEREAL_REGISTER_TYPE(Coffee::MaterialImportData);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::ImportData, Coffee::MaterialImportData);