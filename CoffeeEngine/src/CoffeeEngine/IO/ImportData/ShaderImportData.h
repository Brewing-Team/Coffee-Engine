#pragma once

#include "CoffeeEngine/IO/ImportData/ImportData.h"
#include <cereal/cereal.hpp>

namespace Coffee {

    struct ShaderImportData: public ImportData
    {

        ShaderImportData() : ImportData(ResourceType::Shader) 
        {
            cache = false;
        }

        template<typename Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::base_class<ImportData>(this));
        }
    };

}
CEREAL_REGISTER_TYPE(Coffee::ShaderImportData);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Coffee::ImportData, Coffee::ShaderImportData);