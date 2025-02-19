#pragma once

#include "CoffeeEngine/IO/ImportData/ImportData.h"
#include <cereal/cereal.hpp>
#include <unordered_map>

namespace Coffee {

    struct ModelImportData: public ImportData
    {
        std::unordered_map<std::string, UUID> meshUUIDs;

        template<typename Archive>
        void serialize(Archive& archive)
        {
            archive(cereal::base_class<ImportData>(this), CEREAL_NVP(meshUUIDs));
        }
    };

}